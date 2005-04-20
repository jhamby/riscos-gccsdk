/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strtoumax.c,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <inttypes.h>
#include <stdlib.h>

uintmax_t
strtoumax (const char *nptr, char **endptr, int base)
{
  return strtoull (nptr, endptr, base);
}
