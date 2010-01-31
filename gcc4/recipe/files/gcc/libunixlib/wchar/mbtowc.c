/* mbtowc ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <stdlib.h>

int
mbtowc (wchar_t *pwc, const char *s, size_t n)
{
  if (s == NULL)
    return 0;
  if (n == 0)
    return -1;
  if (pwc)
    *pwc = (wchar_t) * s;
  return *s != '\0';
}
