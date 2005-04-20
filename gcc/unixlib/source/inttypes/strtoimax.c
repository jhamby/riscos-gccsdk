/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strtoimax.c,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <inttypes.h>
#include <stdlib.h>

intmax_t
strtoimax (const char * nptr, char **endptr, int base)
{
  return strtoll (nptr, endptr, base);
}
