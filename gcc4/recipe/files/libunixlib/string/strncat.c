/* strncat ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <string.h>

char *
strncat (char *s, const char *s2, size_t n)
{
  char *s1 = s;

  while (*s1++)
    ;
  --s1;

  while ((*s1++ = *s2++))
    if (!(n--))
      {
	*--s1 = 0;
	break;
      }

  return s;
}
