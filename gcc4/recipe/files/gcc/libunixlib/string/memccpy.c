/* memccpy ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <stddef.h>
#include <string.h>

void *
memccpy (void *dest, const void *src, int c, size_t n)
{
  const char *s = src;
  char *d = dest;

  while (n-- > 0)
    if ((*d++ = *s++) == c)
      return d;

  return NULL;
}
