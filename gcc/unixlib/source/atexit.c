/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/atexit,v $
 * $Date: 1997/10/19 19:19:41 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: atexit,v 1.5 1997/10/19 19:19:41 unixlib Exp $";
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
