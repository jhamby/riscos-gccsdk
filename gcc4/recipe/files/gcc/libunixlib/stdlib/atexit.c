/* atexit ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <stdlib.h>

#include <pthread.h>
#include <internal/unix.h>

int
atexit (void (*atexit_function) (void))
{
  PTHREAD_UNSAFE

  if (__atexit_function_count >= __MAX_ATEXIT_FUNCTION_COUNT)
    return __set_errno (ENOMEM);

  __atexit_function_array[__atexit_function_count++] = atexit_function;
  return 0;
}
