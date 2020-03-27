#include "setattrlist.h"
#include "setattrlistat.h"
#include "../base.h"
#include "../errno.h"
#include "../bsdthread/per_thread_wd.h"
#include "../../../../../platform-include/sys/errno.h"

long sys_setattrlist(const char* path, struct attrlist* alist, void *attributeBuffer, __SIZE_TYPE__ bufferSize, unsigned long options)
{
	return sys_setattrlistat(get_perthread_wd(), path, alist, attributeBuffer, bufferSize, options);
}

