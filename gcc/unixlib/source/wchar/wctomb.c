/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/wchar/wctomb.c,v $
 * $Date: 2001/01/29 15:10:22 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <stdlib.h>

int
wctomb (char *s, wchar_t wchar)
{
  if (s == NULL)
    return 0;

  *s = (char) wchar;
  return 1;
}
