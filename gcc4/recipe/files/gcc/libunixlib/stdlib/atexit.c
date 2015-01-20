/* atexit ()
 * Copyright (c) 2000-2015 UnixLib Developers
 */

#include <errno.h>
#include <stdlib.h>

#include <pthread.h>
#include <internal/unix.h>

/* TODO: When __cxa_atexit is called from a dlopen'd shared library, the
   exit handlers for that library should be called when it is closed and
   removed from the array.
   Perhaps a linked list would be better?  */
int
__cxa_atexit(void (*destructor)(void *),
	     void *arg,
	     void *dso_handle)
{
  PTHREAD_UNSAFE

  if (__atexit_function_count >= __MAX_ATEXIT_FUNCTION_COUNT)
    return __set_errno (ENOMEM);

  __atexit_function_array[__atexit_function_count].func = destructor;
  __atexit_function_array[__atexit_function_count].arg = arg;
  __atexit_function_array[__atexit_function_count].dso_handle = dso_handle;
  __atexit_function_count++;

  return 0;
}

int
atexit (void (*atexit_function) (void))
{
  return __cxa_atexit ((void (*) (void *))atexit_function, NULL, NULL);
}
