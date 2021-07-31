/* strcmp ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <string.h>

int
strcmp (const char *s1, const char *s2)
{
  int i;

  while ((i = *s1) && i == *s2)
    s1++, s2++;

  return *(const unsigned char *) s1 - *(const unsigned char *) s2;
}
