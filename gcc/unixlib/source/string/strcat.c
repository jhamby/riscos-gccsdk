/* strcat ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <string.h>

char *
strcat (char *s, const char *s2)
{
  char *s1 = s;

  while (*s1++)
    ;
  --s1;

  while ((*s1++ = *s2++))
    ;

  return s;
}
