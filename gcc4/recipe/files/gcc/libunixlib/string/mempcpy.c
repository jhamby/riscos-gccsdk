/* mempcpy ()
 * Copyright (c) 2011 UnixLib Developers
 */

#include <string.h>

void *
__mempcpy (void *dest, const void *src, size_t n)
{
  return (char *)memcpy (dest, src, n) + n;
}
weak_alias (__mempcpy, mempcpy)
