/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strtoimax,v $
 * $Date: 2000/01/12 16:55:06 $
 * $Revision: 1.1 $
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
