/* strncasecmp_l ()
 * Copyright (c) 2000-2022 UnixLib Developers
 */

#define __USE_XOPEN2K8
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <locale.h>

int
strncasecmp_l (const char *s1, const char *s2, size_t n, locale_t locobj)
{
  int i, j;

  if (!n)
    return 0;

  do
    {
      i = *s1++, j = *s2++;
      i = tolower_l (i, locobj);
      j = tolower_l (j, locobj);
    }
  while (i && i == j && --n);

  return i - j;
}

