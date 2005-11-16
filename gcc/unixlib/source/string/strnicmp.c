/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/strnicmp.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <string.h>
#include <strings.h>
#include <ctype.h>

int
strnicmp (const char *s1, const char *s2, size_t n)
{
  int i, j;

  if (!n)
    return (0);

  do
    {
      i = *s1++, j = *s2++;
      i = toupper (i);
      j = toupper (j);
    }
  while (i && i == j && --n);

  return (i - j);
}

int
strncasecmp (const char *s1, const char *s2, size_t n)
{
  return strnicmp (s1, s2, n);
}
