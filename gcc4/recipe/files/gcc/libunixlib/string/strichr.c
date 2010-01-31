/* strichr ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *
strichr (const char *s, int c)
{
  int i;

  c = tolower (c);

  do
    {
      i = *s;
      i = tolower (i);
      if (i == c)
	return (char *) s;
    }
  while (*s++);

  return NULL;
}
