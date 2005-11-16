/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/strtok.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <string.h>

/* Defined by POSIX as not threadsafe */ 
char *
strtok (char *s1, const char *s2)

{
  static char *s;

  if (!s1)
    {
      if (!s)
	return (0);
      else
	s1 = s;
    }

  s1 += strspn (s1, s2);

  s = strpbrk (s1, s2);

  if (s)
    *s++ = 0;

  return *s1 ? s1 : NULL;
}
