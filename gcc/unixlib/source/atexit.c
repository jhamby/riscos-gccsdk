/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/atexit.c,v $
 * $Date: 2000/07/15 14:52:08 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: atexit.c,v 1.1.1.1 2000/07/15 14:52:08 nick Exp $";
#endif

#include <stdlib.h>

int
atexit (void (*atexit_function) (void))
{
  if (__atexit_function_count >= __MAX_ATEXIT_FUNCTION_COUNT)
    return -1;

  __atexit_function_array[__atexit_function_count++] = atexit_function;
  return 0;
}
