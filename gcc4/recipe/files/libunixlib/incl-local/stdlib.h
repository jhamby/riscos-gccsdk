/* Internal UnixLib stdlib.h
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#ifndef __STDLIB_H
#include_next <stdlib.h>
#endif

#if !defined(__INTERNAL_STDLIB_H) && defined(__STDLIB_H)
#define __INTERNAL_STDLIB_H

__BEGIN_DECLS

/* Definitions for the atexit array of functions that are to be
   called when the process calls exit.  ANSI requires a minimum
   of 32, however C++ requires 33.  */
#define __MAX_ATEXIT_FUNCTION_COUNT 33

extern void (*__atexit_function_array[__MAX_ATEXIT_FUNCTION_COUNT]) (void);
extern int __atexit_function_count;

__END_DECLS

#endif
