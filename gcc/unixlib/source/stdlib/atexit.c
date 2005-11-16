/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdlib/atexit.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

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
