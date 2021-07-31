/* Internal UnixLib sys/stat.h
 * Copyright (c) 2008-2013 UnixLib Developers
 */

#ifndef __SYS_STAT_H
#include_next <sys/stat.h>
#endif

#if !defined(__INTERNAL_SYS_STAT_H) && defined(__SYS_STAT_H)
#define	__INTERNAL_SYS_STAT_H

__BEGIN_DECLS

int __stat (unsigned __objtype, unsigned __loadaddr, unsigned __execaddr,
	    unsigned __length, unsigned __attr, struct stat *buf);

__END_DECLS

#endif
