/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/strpbrk.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <string.h>

char *
strpbrk (const char *s1, const char *s)
{
  int c1, c2;
  const char *s2;

  while ((c1 = *s1))
    {
      s2 = s;
      while ((c2 = *s2++))
	if (c2 == c1)
	  return (char *) s1;
      s1++;
    }

  return NULL;
}
