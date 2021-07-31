/* strncmp ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <string.h>

int
strncmp (const char *s1, const char *s2, size_t n)
{
  int i;

  if (!n)
    return 0;

  while ((i = *s1) && i == *s2 && --n)
    s1++, s2++;

  return *(const unsigned char *) s1 - *(const unsigned char *) s2;
}
