/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/wchar/mbtowc.c,v $
 * $Date: 2001/01/29 15:10:22 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <stdlib.h>

int
mbtowc (wchar_t *pwc, const char *s, size_t n)
{
  if (s == NULL)
    return 0;
  if (n == 0)
    return -1;
  if (pwc)
    *pwc = (wchar_t) * s;
  return (*s != '\0');
}
