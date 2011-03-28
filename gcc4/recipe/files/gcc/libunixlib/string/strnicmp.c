/* strnicmp (), strncasecmp ()
 * Copyright (c) 2000-2011 UnixLib Developers
 */

#include <string.h>
#include <strings.h>
#include <ctype.h>

int
strnicmp (const char *s1, const char *s2, size_t n)
{
  int i, j;

  if (!n)
    return 0;

  do
    {
      i = *s1++, j = *s2++;
      i = tolower (i);
      j = tolower (j);
    }
  while (i && i == j && --n);

  return i - j;
}
strong_alias (strnicmp, strncasecmp)

