/*
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef __MALLOC_H
#define __MALLOC_H 1
#ifndef __STDLIB_H
#include <stdlib.h>
#endif

__BEGIN_DECLS

extern void *malloc_unlocked (void *state, size_t bytes);
extern void free_unlocked (void *state, void * mem);
extern void *realloc_unlocked (void *state, void *ptr, size_t bytes);
extern void *malloc_trim_unlocked (void *state, size_t pad);

/* Initialisation of malloc.  */
extern void __ul_malloc_init (void);

__END_DECLS

#endif
