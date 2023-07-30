/* Case insensitive string comparison using
   POSIX.1-2008 extended locales.
   Copyright (c) 2002-2022 UnixLib Developers.  */

#define __USE_XOPEN2K8
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <locale.h>

int
strcasecmp_l (const char *s1, const char *s2, locale_t locobj)
{
  const unsigned char *p1 = (const unsigned char *) s1;
  const unsigned char *p2 = (const unsigned char *) s2;
  int result = 0;

  if (p1 == p2)
    return result;

  while (! result)
    {
      result = tolower_l (*p1, locobj) - tolower_l (*p2, locobj);
      if (*p1++ == '\0')
        break;
      p2 ++;
    }

  return result;
}
