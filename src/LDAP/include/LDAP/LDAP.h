/*
 This file is part of Darling.

 Copyright (C) 2019 Lubos Dolezel

 Darling is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Darling is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Darling.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef _LDAP_H_
#define _LDAP_H_

void* ber_alloc(void);
void* ber_alloc_t(void);
void* ber_bprint(void);
void* ber_bvarray_add(void);
void* ber_bvarray_free(void);
void* ber_bvdup(void);
void* ber_bvecadd(void);
void* ber_bvecfree(void);
void* ber_bvfree(void);
void* ber_dump(void);
void* ber_dup(void);
void* ber_dupbv(void);
void* ber_errno_addr(void);
void* ber_error_print(void);
void* ber_first_element(void);
void* ber_flatten(void);
void* ber_flatten2(void);
void* ber_flush(void);
void* ber_free(void);
void* ber_free_buf(void);
void* ber_get_bitstringa(void);
void* ber_get_boolean(void);
void* ber_get_enum(void);
void* ber_get_int(void);
void* ber_get_next(void);
void* ber_get_null(void);
void* ber_get_option(void);
void* ber_get_stringa(void);
void* ber_get_stringal(void);
void* ber_get_stringb(void);
void* ber_get_stringbv(void);
void* ber_get_tag(void);
void* ber_init(void);
void* ber_init2(void);
void* ber_init_w_nullc(void);
void* ber_mem2bv(void);
void* ber_memalloc(void);
void* ber_memcalloc(void);
void* ber_memfree(void);
void* ber_memrealloc(void);
void* ber_memvfree(void);
void* ber_next_element(void);
void* ber_peek_tag(void);
void* ber_printf(void);
void* ber_put_berval(void);
void* ber_put_bitstring(void);
void* ber_put_boolean(void);
void* ber_put_enum(void);
void* ber_put_int(void);
void* ber_put_null(void);
void* ber_put_ostring(void);
void* ber_put_seq(void);
void* ber_put_set(void);
void* ber_put_string(void);
void* ber_read(void);
void* ber_remaining(void);
void* ber_reset(void);
void* ber_scanf(void);
void* ber_set_option(void);
void* ber_skip_tag(void);
void* ber_sockbuf_add_io(void);
void* ber_sockbuf_alloc(void);
void* ber_sockbuf_ctrl(void);
void* ber_sockbuf_free(void);
void* ber_sockbuf_remove_io(void);
void* ber_sos_dump(void);
void* ber_start_seq(void);
void* ber_start_set(void);
void* ber_str2bv(void);
void* ber_strdup(void);
void* ber_write(void);
void* der_alloc(void);
void* ldap_X509dn2bv(void);
void* ldap_abandon(void);
void* ldap_abandon_ext(void);
void* ldap_add(void);
void* ldap_add_ext(void);
void* ldap_add_ext_s(void);
void* ldap_add_result_entry(void);
void* ldap_add_s(void);
void* ldap_attributetype2bv(void);
void* ldap_attributetype2name(void);
void* ldap_attributetype2str(void);
void* ldap_attributetype_free(void);
void* ldap_bind(void);
void* ldap_bind_s(void);
void* ldap_bv2dn(void);
void* ldap_bv2rdn(void);
void* ldap_cancel(void);
void* ldap_cancel_s(void);
void* ldap_compare(void);
void* ldap_compare_ext(void);
void* ldap_compare_ext_s(void);
void* ldap_compare_s(void);
void* ldap_contentrule2bv(void);
void* ldap_contentrule2name(void);
void* ldap_contentrule2str(void);
void* ldap_contentrule_free(void);
void* ldap_control_create(void);
void* ldap_control_find(void);
void* ldap_control_free(void);
void* ldap_controls_free(void);
void* ldap_count_entries(void);
void* ldap_count_messages(void);
void* ldap_count_references(void);
void* ldap_count_values(void);
void* ldap_count_values_len(void);
void* ldap_create(void);
void* ldap_create_assertion_control_value(void);
void* ldap_create_control(void);
void* ldap_create_page_control(void);
void* ldap_create_page_control_value(void);
void* ldap_create_sort_control(void);
void* ldap_create_sort_keylist(void);
void* ldap_create_vlv_control(void);
void* ldap_dcedn2dn(void);
void* ldap_delete(void);
void* ldap_delete_ext(void);
void* ldap_delete_ext_s(void);
void* ldap_delete_result_entry(void);
void* ldap_delete_s(void);
void* ldap_dn2ad_canonical(void);
void* ldap_dn2bv(void);
void* ldap_dn2dcedn(void);
void* ldap_dn2domain(void);
void* ldap_dn2str(void);
void* ldap_dn2ufn(void);
void* ldap_dn_normalize(void);
void* ldap_dnfree(void);
void* ldap_domain2dn(void);
void* ldap_domain2hostlist(void);
void* ldap_err2string(void);
void* ldap_explode_dn(void);
void* ldap_explode_rdn(void);
void* ldap_extended_operation(void);
void* ldap_extended_operation_s(void);
void* ldap_find_control(void);
void* ldap_first_attribute(void);
void* ldap_first_entry(void);
void* ldap_first_message(void);
void* ldap_first_reference(void);
void* ldap_free_sort_keylist(void);
void* ldap_free_urldesc(void);
void* ldap_get_attribute_ber(void);
void* ldap_get_dn(void);
void* ldap_get_dn_ber(void);
void* ldap_get_entry_controls(void);
void* ldap_get_option(void);
void* ldap_get_values(void);
void* ldap_get_values_len(void);
void* ldap_init(void);
void* ldap_initialize(void);
void* ldap_is_ldap_url(void);
void* ldap_is_ldapi_url(void);
void* ldap_is_ldaps_url(void);
void* ldap_matchingrule2bv(void);
void* ldap_matchingrule2name(void);
void* ldap_matchingrule2str(void);
void* ldap_matchingrule_free(void);
void* ldap_matchingruleuse2bv(void);
void* ldap_matchingruleuse2name(void);
void* ldap_matchingruleuse2str(void);
void* ldap_matchingruleuse_free(void);
void* ldap_memalloc(void);
void* ldap_memcalloc(void);
void* ldap_memfree(void);
void* ldap_memrealloc(void);
void* ldap_memvfree(void);
void* ldap_modify(void);
void* ldap_modify_ext(void);
void* ldap_modify_ext_s(void);
void* ldap_modify_s(void);
void* ldap_modrdn(void);
void* ldap_modrdn2(void);
void* ldap_modrdn2_s(void);
void* ldap_modrdn_s(void);
void* ldap_mods_free(void);
void* ldap_msgdelete(void);
void* ldap_msgfree(void);
void* ldap_msgid(void);
void* ldap_msgtype(void);
void* ldap_nameform2bv(void);
void* ldap_nameform2name(void);
void* ldap_nameform2str(void);
void* ldap_nameform_free(void);
void* ldap_next_attribute(void);
void* ldap_next_entry(void);
void* ldap_next_message(void);
void* ldap_next_reference(void);
void* ldap_objectclass2bv(void);
void* ldap_objectclass2name(void);
void* ldap_objectclass2str(void);
void* ldap_objectclass_free(void);
void* ldap_open(void);
void* ldap_parse_extended_result(void);
void* ldap_parse_intermediate(void);
void* ldap_parse_page_control(void);
void* ldap_parse_pageresponse_control(void);
void* ldap_parse_passwd(void);
void* ldap_parse_reference(void);
void* ldap_parse_result(void);
void* ldap_parse_sasl_bind_result(void);
void* ldap_parse_sort_control(void);
void* ldap_parse_sortresponse_control(void);
void* ldap_parse_vlv_control(void);
void* ldap_parse_vlvresponse_control(void);
void* ldap_parse_whoami(void);
void* ldap_passwd(void);
void* ldap_passwd_s(void);
void* ldap_perror(void);
void* ldap_put_vrFilter(void);
void* ldap_rdn2bv(void);
void* ldap_rdn2str(void);
void* ldap_rdnfree(void);
void* ldap_rename(void);
void* ldap_rename2(void);
void* ldap_rename2_s(void);
void* ldap_rename_s(void);
void* ldap_result(void);
void* ldap_result2error(void);
void* ldap_sasl_bind(void);
void* ldap_sasl_bind_s(void);
void* ldap_sasl_interactive_bind_s(void);
void* ldap_scherr2str(void);
void* ldap_search(void);
void* ldap_search_ext(void);
void* ldap_search_ext_s(void);
void* ldap_search_s(void);
void* ldap_search_st(void);
void* ldap_set_option(void);
void* ldap_set_rebind_proc(void);
void* ldap_set_search_results_callback(void);
void* ldap_simple_bind(void);
void* ldap_simple_bind_s(void);
void* ldap_sort_entries(void);
void* ldap_sort_strcasecmp(void);
void* ldap_sort_values(void);
void* ldap_start_tls_s(void);
void* ldap_str2attributetype(void);
void* ldap_str2contentrule(void);
void* ldap_str2dn(void);
void* ldap_str2matchingrule(void);
void* ldap_str2matchingruleuse(void);
void* ldap_str2nameform(void);
void* ldap_str2objectclass(void);
void* ldap_str2rdn(void);
void* ldap_str2structurerule(void);
void* ldap_str2syntax(void);
void* ldap_strdup(void);
void* ldap_structurerule2bv(void);
void* ldap_structurerule2name(void);
void* ldap_structurerule2str(void);
void* ldap_structurerule_free(void);
void* ldap_syntax2bv(void);
void* ldap_syntax2name(void);
void* ldap_syntax2str(void);
void* ldap_syntax_free(void);
void* ldap_unbind(void);
void* ldap_unbind_ext(void);
void* ldap_unbind_ext_s(void);
void* ldap_unbind_s(void);
void* ldap_url_desc2str(void);
void* ldap_url_parse(void);
void* ldap_value_free(void);
void* ldap_value_free_len(void);
void* ldap_whoami(void);
void* ldap_whoami_s(void);
void* ldap_x_mb_to_utf8(void);
void* ldap_x_mbs_to_utf8s(void);
void* ldap_x_ucs4_to_utf8(void);
void* ldap_x_utf8_to_mb(void);
void* ldap_x_utf8_to_ucs4(void);
void* ldap_x_utf8_to_wc(void);
void* ldap_x_utf8s_to_mbs(void);
void* ldap_x_utf8s_to_wcs(void);
void* ldap_x_wc_to_utf8(void);
void* ldap_x_wcs_to_utf8s(void);
void* ldif_close(void);
void* ldif_countlines(void);
void* ldif_fetch_url(void);
void* ldif_getline(void);
void* ldif_is_not_printable(void);
void* ldif_must_b64_encode_register(void);
void* ldif_must_b64_encode_release(void);
void* ldif_open(void);
void* ldif_open_url(void);
void* ldif_parse_line(void);
void* ldif_parse_line2(void);
void* ldif_put(void);
void* ldif_put_wrap(void);
void* ldif_read_record(void);
void* ldif_sput(void);
void* ldif_sput_wrap(void);

#endif
