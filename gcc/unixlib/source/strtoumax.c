/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strtoumax.c,v $
 * $Date: 2000/07/15 14:52:10 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <inttypes.h>
#include <stdlib.h>

uintmax_t
strtoumax (const char *nptr, char **endptr, int base)
{
  return strtoull (nptr, endptr, base);
}
