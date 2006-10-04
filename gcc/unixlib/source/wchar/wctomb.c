/* wctomb ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <stdlib.h>

int
wctomb (char *s, wchar_t wchar)
{
  if (s == NULL)
    return 0;

  *s = (char) wchar;
  return 1;
}
