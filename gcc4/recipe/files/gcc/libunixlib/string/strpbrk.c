/* strpbrk ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

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
