/* Internal UnixLib sys/stat.h
 * Copyright (c) 2008 UnixLib Developers
 */

#ifndef __SYS_STAT_H
#include_next <sys/stat.h>
#endif

#if !defined(__INTERNAL_SYS_STAT_H) && defined(__SYS_STAT_H)
#define	__INTERNAL_SYS_STAT_H

__BEGIN_DECLS

/* We declare __stat here to prevent the inclusion if <sys/stat.h> in
   <unixlib/local.h>.  This function is internal to stat and fstat.  */
extern void __stat (int __objtype, int __loadaddr, int __execaddr,
                    int __length, int __attr, struct stat *buf);

__END_DECLS

#endif
