/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strtoimax.c,v $
 * $Date: 2000/07/15 14:52:09 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <inttypes.h>
#include <stdlib.h>

intmax_t
strtoimax (const char * nptr, char **endptr, int base)
{
  return strtoll (nptr, endptr, base);
}
