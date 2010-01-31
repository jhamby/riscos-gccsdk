/* Internal UnixLib unistd.h
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#ifndef __UNISTD_H
#include_next <unistd.h>
#endif

#if !defined(__INTERNAL_UNISTD_H) && defined(__UNISTD_H)
#define __INTERNAL_UNISTD_H

__BEGIN_DECLS

/* Removes the suffix swap directory if it is empty.
   The filename passed to it is corrupted. */
extern void __unlinksuffix (char *__file);

extern void *__internal_sbrk (int __incr);

__END_DECLS

#endif
