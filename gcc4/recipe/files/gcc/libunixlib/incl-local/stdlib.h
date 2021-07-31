/* Internal UnixLib stdlib.h
 * Copyright (c) 2000-2015 UnixLib Developers
 */

#ifndef __STDLIB_H
#include_next <stdlib.h>
#endif

#if !defined(__INTERNAL_STDLIB_H) && defined(__STDLIB_H)
#define __INTERNAL_STDLIB_H

__BEGIN_DECLS

/* Register a function to be called by exit or when a shared
   library is unloaded.  */
extern int
__cxa_atexit(void (*destructor)(void *), void *arg, void *dso_handle);

extern void
__cxa_finalize(void *);

__END_DECLS

#endif
