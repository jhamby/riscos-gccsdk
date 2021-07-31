/* strrchr ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <string.h>

char *
strrchr (const char *s, int c_)
{
  int i;
  const char *_s;
  char c = (char) c_;

  _s = 0;
  do
    if ((i = *s) == c)
      _s = s;
  while (s++, i);

  return ((char *) _s);
}
strong_alias (strrchr, rindex)
