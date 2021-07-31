/* strncpy ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <string.h>

char *
strncpy (char *s, const char *s2, size_t n)
{
  char *s1 = s;

  while (n--)
    if (!(*s1++ = *s2++))
      {
	while (n--)
	  *s1++ = 0;
	return s;
      }

  return s;
}
