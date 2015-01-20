/* Internal UnixLib stdlib.h
 * Copyright (c) 2000-2015 UnixLib Developers
 */

#ifndef __STDLIB_H
#include_next <stdlib.h>
#endif

#if !defined(__INTERNAL_STDLIB_H) && defined(__STDLIB_H)
#define __INTERNAL_STDLIB_H

struct __atexit_function_entry
{
  void (*func)(void *);
  void *arg;
  void *dso_handle;
};

__BEGIN_DECLS

/* Definitions for the atexit array of functions that are to be
   called when the process calls exit.  ANSI requires a minimum
   of 32, however C++ requires 33.  */
#define __MAX_ATEXIT_FUNCTION_COUNT 33

extern struct __atexit_function_entry __atexit_function_array[__MAX_ATEXIT_FUNCTION_COUNT];
extern int __atexit_function_count;

/* Register a function to be called by exit or when a shared
   library is unloaded.  */
extern int
__cxa_atexit(void (*destructor)(void *), void *arg, void *dso_handle);

__END_DECLS

#endif
