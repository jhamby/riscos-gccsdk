/* strtok ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <string.h>

/* Defined by POSIX as not threadsafe */ 
char *
strtok (char *s1, const char *s2)
{
  static char *s;

  if (!s1)
    {
      if (!s)
	return NULL;

      s1 = s;
    }

  s1 += strspn (s1, s2);

  s = strpbrk (s1, s2);

  if (s)
    *s++ = 0;

  return *s1 ? s1 : NULL;
}
