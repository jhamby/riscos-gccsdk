/* strstr ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <string.h>

char *
strstr (const char *s1, const char *s2)

{
  size_t l1 = strlen (s1), l2 = strlen (s2);
  const char *e1 = s1 + l1 - l2;

  while (s1 <= e1)
    {
      if (!strncmp (s1, s2, l2))
	return ((char *) s1);
      s1++;
    }

  return (0);
}
