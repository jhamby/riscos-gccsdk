/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/wchar/mblen.c,v $
 * $Date: 2001/01/29 15:10:22 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <stdlib.h>

int
mblen (const char *s, size_t n)
{
  if (s == NULL || *s == '\0')
    return 0;
  if (n == 0)
    return -1;
  return 1;
}
