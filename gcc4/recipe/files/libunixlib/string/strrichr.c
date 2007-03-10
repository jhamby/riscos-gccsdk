/* strrichr ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <string.h>
#include <ctype.h>

char *
strrichr (const char *s, int c)
{
  int i;
  const char *_s;

  c = tolower (c);

  _s = 0;
  do
    {
      i = *s;
      i = tolower (i);
      if (i == c)
	_s = s;
    }
  while (s++, i);

  return ((char *) _s);
}
