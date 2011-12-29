/* wmempcpy ()
 * Copyright (c) 2011 UnixLib Developers
 */

#include <string.h>
#include <wchar.h>

wchar_t *
__wmempcpy (wchar_t *dest, const wchar_t *src, size_t n)
{
  return wmemcpy (dest, src, n) + n;
}
weak_alias (__wmempcpy, wmempcpy)
