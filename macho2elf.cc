// Copyright 2011 Shinichiro Hamaji. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//   1. Redistributions of source code must retain the above copyright
//      notice, this list of  conditions and the following disclaimer.
//
//   2. Redistributions in binary form must reproduce the above
//      copyright notice, this list of conditions and the following
//      disclaimer in the documentation and/or other materials
//      provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY Shinichiro Hamaji ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Shinichiro Hamaji OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
// USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// An attempt to translate from Mach-O to ELF.
//
// Note that programs generated by this program won't run because this
// doesn't initialize glibc properly.

#include <assert.h>
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <map>
#include <string>
#include <vector>

#include "mach-o.h"

using namespace std;

static map<string, string> g_rename;
static vector<string> g_sos;

static void initRename() {
#define RENAME(src, dst) g_rename.insert(make_pair(#src, #dst));
#define WRAP(src) RENAME(src, __darwin_ ## src)
#include "rename.tab"
#undef RENAME
#undef WRAP
}

static uint64_t alignMem(uint64_t p, uint64_t a) {
  a--;
  return (p + a) & ~a;
}

template <bool is64>
struct BitsHelpers {
  typedef Elf64_Ehdr Elf_Ehdr;
  typedef Elf64_Phdr Elf_Phdr;
  typedef Elf64_Shdr Elf_Shdr;
  typedef Elf64_Dyn Elf_Dyn;
  typedef Elf64_Sym Elf_Sym;
  typedef Elf64_Rela Elf_Rel;
  typedef uint64_t intptr;
  typedef segment_command_64 mach_segment;

  static const vector<mach_segment*>& segments(const MachO& mach) {
    return mach.segments64();
  }

  static int elf_st_bind(int val) {
    return ELF64_ST_BIND(val);
  }
};

template <>
struct BitsHelpers<false> {
  typedef Elf32_Ehdr Elf_Ehdr;
  typedef Elf32_Phdr Elf_Phdr;
  typedef Elf32_Shdr Elf_Shdr;
  typedef Elf32_Dyn Elf_Dyn;
  typedef Elf32_Sym Elf_Sym;
  typedef Elf32_Rela Elf_Rel;
  typedef uint32_t intptr;
  typedef segment_command mach_segment;

  static const vector<mach_segment*>& segments(const MachO& mach) {
    return mach.segments();
  }

  static int elf_st_bind(int val) {
    return ELF32_ST_BIND(val);
  }
};

template <bool is64>
class ELFBuilder {
  typedef BitsHelpers<is64> Helpers;
  typedef typename Helpers::Elf_Ehdr Ehdr;
  typedef typename Helpers::Elf_Phdr Phdr;
  typedef typename Helpers::Elf_Shdr Shdr;
  typedef typename Helpers::Elf_Dyn Dyn;
  typedef typename Helpers::Elf_Sym Sym;
  typedef typename Helpers::Elf_Rel Rel;
  typedef typename Helpers::intptr intptr;
  typedef typename Helpers::mach_segment Segment;

 public:
  ~ELFBuilder() {
    for (size_t i = 0; i < sections_.size(); i++) {
      delete sections_[i];
    }

    fclose(fp_);
  }

  void emit(const MachO& mach, const char* filename) {
    Section* null_section = newSection("", SHT_NULL);
    null_section->flags = 0;

    fp_ = fopen(filename, "wb");
    if (!fp_) {
      fprintf(stderr, "Cannot write %s\n", filename);
      exit(1);
    }

    int num_phdr = 3;  // PT_INTERP + PT_DYNAMIC + PT_LOAD for dynamic
    intptr base_vaddr = 0;
    intptr max_vaddr = 0;

    const vector<Segment*>& segments = Helpers::segments(mach);
    for (size_t i = 0; i < segments.size(); i++) {
      Segment* seg = segments[i];
      const char* name = seg->segname;
      if (!strcmp(name, SEG_PAGEZERO)) {
        continue;
      }
      if (!strcmp(name, SEG_TEXT)) {
        base_vaddr = seg->vmaddr;
      }
      max_vaddr = max(max_vaddr, seg->vmaddr + seg->vmsize);
      num_phdr++;
    }

    Ehdr ehdr;
    memset(&ehdr, 0, sizeof(ehdr));
    ehdr.e_ident[EI_MAG0] = ELFMAG0;
    ehdr.e_ident[EI_MAG1] = ELFMAG1;
    ehdr.e_ident[EI_MAG2] = ELFMAG2;
    ehdr.e_ident[EI_MAG3] = ELFMAG3;
    ehdr.e_ident[EI_CLASS] = is64 ? ELFCLASS64 : ELFCLASS32;
    ehdr.e_ident[EI_DATA] = ELFDATA2LSB;
    ehdr.e_ident[EI_VERSION] = EV_CURRENT;
    ehdr.e_ident[EI_OSABI] = ELFOSABI_SYSV;
    ehdr.e_type = ET_EXEC;
    ehdr.e_machine = is64 ? EM_X86_64 : EM_386;
    ehdr.e_version = EV_CURRENT;
    ehdr.e_entry = mach.entry();
    ehdr.e_phoff = sizeof(ehdr);
    ehdr.e_shoff = 0;
    ehdr.e_flags = 0;
    ehdr.e_ehsize = sizeof(ehdr);
    ehdr.e_phentsize = sizeof(Phdr);
    ehdr.e_phnum = num_phdr;
    ehdr.e_shentsize = sizeof(Shdr);
    ehdr.e_shnum = 0;
    ehdr.e_shstrndx = 0;
    fwrite(&ehdr, sizeof(ehdr), 1, fp_);

    intptr offset = sizeof(Ehdr) + sizeof(Phdr) * num_phdr;
    Phdr phdr;

    const char* loader =
      is64 ? "/lib64/ld-linux-x86-64.so.2" : "/lib/ld-linux.so.2";
    phdr.p_type = PT_INTERP;
    phdr.p_offset = offset;
    phdr.p_vaddr = base_vaddr + phdr.p_offset;
    phdr.p_paddr = phdr.p_vaddr;
    phdr.p_filesz = strlen(loader) + 1;
    phdr.p_memsz = phdr.p_filesz;
    phdr.p_flags = PF_R;
    phdr.p_align = 1;
    fwrite(&phdr, sizeof(phdr), 1, fp_);

    offset += phdr.p_filesz;

    for (size_t i = 0; i < segments.size(); i++) {
      Segment* seg = segments[i];
      const char* name = seg->segname;
      if (!strcmp(name, SEG_PAGEZERO)) {
        continue;
      }

      phdr.p_type = PT_LOAD;
      phdr.p_offset = seg->fileoff;
      phdr.p_vaddr = seg->vmaddr;
      phdr.p_paddr = phdr.p_vaddr;
      // TODO!
      //phdr.p_filesz = max<uint64_t>(0x1000, seg->filesize);
      phdr.p_filesz = alignMem(seg->filesize, 0x1000);
      phdr.p_memsz = seg->vmsize;
      phdr.p_flags = 0;
      if (seg->initprot & VM_PROT_READ) {
        phdr.p_flags |= PF_R;
      }
      if (seg->initprot & VM_PROT_WRITE) {
        phdr.p_flags |= PF_W;
      }
      if (seg->initprot & VM_PROT_EXECUTE) {
        phdr.p_flags |= PF_X;
      }
      phdr.p_align = 0x1000;
      fwrite(&phdr, sizeof(phdr), 1, fp_);

      const char* sec_name = seg->segname;
      int flags = SHF_ALLOC;
      int align = 8;
      //size = phdr.p_filesz;
      uint64_t size = seg->filesize;
      if (!strcmp(sec_name, SEG_TEXT)) {
        sec_name = ".text";
        flags |= SHF_EXECINSTR;
        align = 16;
        // TODO: maybe OK?
        size = alignMem(size, 0x1000) - offset;
        seg->fileoff = offset;
        seg->filesize = size;
      } else if (!strcmp(sec_name, SEG_DATA)) {
        sec_name = ".data";
        flags |= SHF_WRITE;
      }
      Section* sec = newSection(sec_name, SHT_PROGBITS);
      sec->override_data_size = phdr.p_filesz;
      sec->vmaddr = seg->vmaddr;
      sec->offset = phdr.p_offset;
      sec->flags |= flags;
      sec->align = align;

      offset += size;
    }

    size_t num_dyns = 9 + g_sos.size();

    intptr dynamic_offset = max_vaddr + (offset & 0xfff);

    phdr.p_type = PT_LOAD;
    phdr.p_offset = offset;
    phdr.p_vaddr = dynamic_offset;
    phdr.p_paddr = phdr.p_vaddr;
    // TODO: need to rewrite the size
    phdr.p_filesz = 0x4000;
    phdr.p_memsz = phdr.p_filesz;
    phdr.p_flags = PF_R | PF_W;
    phdr.p_align = 8;
    fwrite(&phdr, sizeof(phdr), 1, fp_);

    phdr.p_type = PT_DYNAMIC;
    phdr.p_filesz = sizeof(Dyn) * num_dyns;
    fwrite(&phdr, sizeof(phdr), 1, fp_);

    Section* dynamic = newSection(".dynamic", SHT_DYNAMIC);
    dynamic->entsize = sizeof(Dyn);
    dynamic->offset = phdr.p_offset;
    dynamic->override_data_size = phdr.p_filesz;
    dynamic->flags |= SHF_WRITE;

    offset += phdr.p_filesz;
    dynamic_offset += phdr.p_filesz;

    fwrite(loader, strlen(loader) + 1, 1, fp_);

    for (size_t i = 0; i < segments.size(); i++) {
      Segment* seg = segments[i];
      const char* name = seg->segname;
      if (!strcmp(name, SEG_PAGEZERO)) {
        continue;
      }

      fwrite(mach.base() + seg->fileoff, 1, seg->filesize, fp_);
    }

    Symtab* symtab = newSymtab(".dynsym", SHT_DYNSYM, ".dynstr", ".hash");
    dynamic->link = symtab->str;

    vector<Rel> rels;
    for (size_t i = 0; i < mach.binds().size(); i++) {
      MachO::Bind* bind = mach.binds()[i];

      if (bind->name[0] != '_') {
        continue;
      }

      if (bind->type == BIND_TYPE_POINTER) {
        const char* name = bind->name + 1;

        printf("Putting ELF symbol: %s\n", name);

        map<string, string>::const_iterator found =
          g_rename.find(name);
        if (found != g_rename.end()) {
          printf("Applying renaming: %s => %s\n",
                 name, found->second.c_str());
          name = found->second.c_str();
        }

        int sym_index = putELFSym(symtab, bind->vmaddr, 0,
                                  ELF64_ST_INFO(STB_GLOBAL, STT_FUNC),
                                  0, 0, name);

        Rel rel;
        rel.r_offset = bind->vmaddr;
        rel.r_info = ELF64_R_INFO(sym_index, R_X86_64_JUMP_SLOT);
        rel.r_addend = bind->addend;
        rels.push_back(rel);
      } else {
        printf("Unknown bind type: %d\n", bind->type);
        abort();
      }
    }

    vector<Dyn> dyns;
    for (size_t i = 0; i < g_sos.size(); i++) {
      addDynVal(DT_NEEDED, putELFStr(symtab->str, g_sos[i].c_str()),
                &dyns);
    }

    addDynVal(DT_HASH, dynamic_offset, &dyns);
    symtab->hash->offset = offset;
    offset += symtab->hash->data.size();
    dynamic_offset += symtab->hash->data.size();

    addDynVal(DT_SYMTAB, dynamic_offset, &dyns);
    symtab->sym->offset = offset;
    offset += symtab->sym->data.size();
    dynamic_offset += symtab->sym->data.size();

    addDynVal(DT_STRTAB, dynamic_offset, &dyns);
    symtab->str->offset = offset;
    offset += symtab->str->data.size();
    dynamic_offset += symtab->str->data.size();

    addDynVal(DT_STRSZ, symtab->str->data.size(), &dyns);
    addDynVal(DT_SYMENT, sizeof(Sym), &dyns);
    addDynVal(DT_RELA, dynamic_offset, &dyns);

    addDynVal(DT_RELASZ, sizeof(Rel) * rels.size(), &dyns);
    Section* rel_sec = newSection(".rela.got", SHT_RELA);
    rel_sec->offset = offset;
    rel_sec->override_data_size = sizeof(Rel) * rels.size();
    rel_sec->entsize = sizeof(Rel);
    rel_sec->link = symtab->sym;
    offset += sizeof(Rel) * rels.size();

    addDynVal(DT_RELAENT, sizeof(Rel), &dyns);
    addDynVal(DT_NULL, 0, &dyns);

    assert(num_dyns == dyns.size());

    for (size_t i = 0; i < dyns.size(); i++) {
      fwrite(&dyns[i], 1, sizeof(Dyn), fp_);
    }
    fwrite(symtab->hash->data.data(), symtab->hash->data.size(), 1, fp_);
    fwrite(symtab->sym->data.data(), symtab->sym->data.size(), 1, fp_);
    fwrite(symtab->str->data.data(), symtab->str->data.size(), 1, fp_);
    for (size_t i = 0; i < rels.size(); i++) {
      fwrite(&rels[i], 1, sizeof(Rel), fp_);
    }

    delete symtab;

    Section* shstrtab = newSection(".shstrtab", SHT_STRTAB);
    shstrtab->offset = offset;

    for (size_t i = 0; i < sections_.size(); i++) {
      Section* sec = sections_[i];
      sec->name_offset = putELFStr(shstrtab, sec->name.c_str());
    }

    fwrite(shstrtab->data.data(), shstrtab->data.size(), 1, fp_);
    offset += shstrtab->data.size();

    ehdr.e_shoff = offset;
    ehdr.e_shstrndx = sections_.size() - 1;

    for (size_t i = 0; i < sections_.size(); i++) {
      Section* sec = sections_[i];
      Shdr shdr;
      shdr.sh_name = sec->name_offset;
      shdr.sh_type = sec->type;
      shdr.sh_flags = sec->flags;
      shdr.sh_addr = sec->offset + base_vaddr;
      if (sec->vmaddr) {
        shdr.sh_addr = sec->vmaddr;
      }
      shdr.sh_offset = sec->offset;
      shdr.sh_size = sec->data.size();
      if (sec->override_data_size) {
        shdr.sh_size = sec->override_data_size;
      }
      shdr.sh_link = 0;
      if (sec->link) {
        shdr.sh_link = sec->link->index;
      }
      shdr.sh_info = 0;
      //shdr.sh_addralign = sec->align;
      shdr.sh_addralign = 0;
      shdr.sh_entsize = sec->entsize;
      fwrite(&shdr, 1, sizeof(shdr), fp_);
    }
    ehdr.e_shnum = sections_.size();

    char padding[4096];
    fwrite(padding, 1, sizeof(padding), fp_);

    fseek(fp_, 0, SEEK_SET);
    fwrite(&ehdr, sizeof(ehdr), 1, fp_);
  }

 private:
  struct Section {
    string name;
    int name_offset;
    string data;
    uint64_t offset;
    uint64_t vmaddr;
    uint64_t override_data_size;
    uint32_t type;
    int nb_hashed_syms;
    int entsize;
    int index;
    int flags;
    int align;
    Section* link;
  };

  struct Symtab {
    Section* sym;
    Section* str;
    Section* hash;

    ~Symtab() {
    }
  };

  static char* addSectionPtr(Section* sec, uint64_t size) {
    size_t offset = sec->data.size();
    sec->data.resize(offset + size);
    return &sec->data[offset];
  }

  Section* newSection(const char *name, uint32_t type) {
    Section* sec;
    sec = new Section();
    sec->name = name;
    sec->type = type;
    sec->flags = SHF_ALLOC;
    sec->index = static_cast<int>(sections_.size());

    sec->name_offset = 0;
    sec->offset = 0;
    sec->vmaddr = 0;
    sec->override_data_size = 0;
    sec->nb_hashed_syms = 0;
    sec->entsize = 0;
    sec->align = 0;
    sec->link = NULL;

    sections_.push_back(sec);
    return sec;
  }

  Symtab* newSymtab(const char* symtab_name, uint32_t symtab_type,
                    const char* strtab_name,
                    const char* hash_name) {
    Symtab* symtab = new Symtab();
    symtab->sym = newSection(symtab_name, symtab_type);
    symtab->sym->entsize = sizeof(Sym);
    symtab->str = newSection(strtab_name, SHT_STRTAB);
    putELFStr(symtab->str, "");
    symtab->sym->link = symtab->str;
    putELFSym(symtab, 0, 0, 0, 0, 0, NULL);

    int nb_buckets = 1;
    symtab->hash = newSection(hash_name, SHT_HASH);
    symtab->hash->entsize = sizeof(int);
    symtab->hash->link = symtab->sym;

    int* ptr = (int*)addSectionPtr(symtab->hash,
                                   (2 + nb_buckets + 1) * sizeof(int));
    ptr[0] = nb_buckets;
    ptr[1] = 1;
    memset(ptr + 2, 0, (nb_buckets + 1) * sizeof(int));

    return symtab;
  }

  /* return the symbol number */
  static int putELFSym(Symtab* symtab,
                       unsigned long value, unsigned long size,
                       int info, int other, int shndx, const char *name) {
    int name_offset, sym_index;
    int nbuckets, h;
    Sym* sym;
    Section* hs;
    Section* s = symtab->sym;

    sym = reinterpret_cast<Sym*>(addSectionPtr(s, sizeof(Sym)));
    if (name) {
      name_offset = putELFStr(symtab->str, name);
    } else {
      name_offset = 0;
    }
    /* XXX: endianness */
    sym->st_name = name_offset;
    sym->st_value = value;
    sym->st_size = size;
    sym->st_info = info;
    sym->st_other = other;
    sym->st_shndx = shndx;
    sym_index = sym - (Sym*)s->data.data();
    printf("sym: %x %p index=%d\n",
           sym->st_name, (void*)sym->st_value, sym_index);
    hs = symtab->hash;
    if (hs) {
      int *ptr, *base;
      ptr = reinterpret_cast<int*>(addSectionPtr(hs, sizeof(int)));
      base = (int*)hs->data.data();
      /* only add global or weak symbols */
      if (Helpers::elf_st_bind(info) != STB_LOCAL) {
        /* add another hashing entry */
        nbuckets = base[0];
        h = calcHash(name) % nbuckets;
        *ptr = base[2 + h];
        base[2 + h] = sym_index;
        base[1]++;
        /* we resize the hash table */
        hs->nb_hashed_syms++;
        if (hs->nb_hashed_syms > 2 * nbuckets) {
          rebuildHash(symtab, 2 * nbuckets);
        }
      } else {
        *ptr = 0;
        base[1]++;
      }
    }
    return sym_index;
  }

  /* rebuild hash table of section s */
  /* NOTE: we do factorize the hash table code to go faster */
  static void rebuildHash(Symtab* symtab, unsigned int nb_buckets) {
    Sym* sym;
    int* ptr;
    int* hash;
    int nb_syms, sym_index, h;
    char* strtab;
    Section* s = symtab->sym;

    strtab = (char*)symtab->str->data.data();
    nb_syms = s->data.size() / sizeof(Sym);

    symtab->hash->data.clear();
    ptr = (int*)addSectionPtr(symtab->hash,
                              (2 + nb_buckets + nb_syms) * sizeof(int));
    ptr[0] = nb_buckets;
    ptr[1] = nb_syms;
    ptr += 2;
    hash = ptr;
    memset(hash, 0, (nb_buckets + 1) * sizeof(int));
    ptr += nb_buckets + 1;

    sym = (Sym*)s->data.data() + 1;
    for(sym_index = 1; sym_index < nb_syms; sym_index++) {
      if (Helpers::elf_st_bind(sym->st_info) != STB_LOCAL) {
        h = calcHash(strtab + sym->st_name) % nb_buckets;
        *ptr = hash[h];
        hash[h] = sym_index;
      } else {
        *ptr = 0;
      }
      ptr++;
      sym++;
    }
  }

  static uint64_t putELFStr(Section* sec, const char* sym) {
    uint64_t offset = sec->data.size();
    sec->data += sym;
    sec->data += '\0';
    return offset;
  }

  static void addDynVal(int64_t tag, uint64_t val, vector<Dyn>* dyns) {
    Dyn dyn;
    dyn.d_tag = tag;
    dyn.d_un.d_val = val;
    dyns->push_back(dyn);
  }

  static void addDynPtr(int64_t tag, uint64_t ptr, vector<Dyn>* dyns) {
    Dyn dyn;
    dyn.d_tag = tag;
    dyn.d_un.d_ptr = ptr;
    dyns->push_back(dyn);
  }

  static uint64_t addStr(const string& s, string* o) {
    uint64_t r = s.size();
    *o += s;
    *o += '\0';
    return r;
  }

  static uint32_t calcHash(const char* name) {
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(name);
    uint32_t h = 0;
    while (*ptr) {
      h <<= 4;
      h += *ptr++;
      uint32_t g = h & 0xf0000000;
      h ^= g >> 24;
      h &= ~g;
    }
    return h;
  }

  FILE* fp_;
  vector<Section*> sections_;
};

template <bool is64>
void emitELF(const MachO& mach, const char* filename) {
  ELFBuilder<is64> builder;
  builder.emit(mach, filename);
}

int main(int argc, char* argv[]) {
  initRename();

  vector<string> args;
  g_sos.push_back("/lib/libc.so.6");
  for (int i = 1; i < argc; i++) {
    const char* arg = argv[i];
    if (arg[0] != '-') {
      args.push_back(arg);
      continue;
    }

    if (arg[1] == 'l') {
      g_sos.push_back(arg + 2);
    } else {
      fprintf(stderr, "Unknown switch '%s'\n", arg);
      exit(1);
    }
  }

  if (args.size() < 2) {
    exit(1);
  }

  MachO mach(args[0].c_str());
  if (mach.is64()) {
    emitELF<true>(mach, args[1].c_str());
  } else {
    emitELF<false>(mach, args[1].c_str());
  }
}
