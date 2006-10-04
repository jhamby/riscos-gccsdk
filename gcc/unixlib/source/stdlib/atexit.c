/* atexit ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <stdlib.h>
#include <pthread.h>

int
atexit (void (*atexit_function) (void))
{
  PTHREAD_UNSAFE

  if (__atexit_function_count >= __MAX_ATEXIT_FUNCTION_COUNT)
    return -1;

  __atexit_function_array[__atexit_function_count++] = atexit_function;
  return 0;
}
