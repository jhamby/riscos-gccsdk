/* Case insensitive string comparison.
   Copyright (c) 2002, 2003, 2004, 2005, 2008 UnixLib Developers.  */

#include <string.h>
#include <strings.h>
#include <ctype.h>

int
stricmp (const char *s1, const char *s2)
{
  const unsigned char *p1 = (const unsigned char *) s1;
  const unsigned char *p2 = (const unsigned char *) s2;
  int result = 0;

  if (p1 == p2)
    return result;

  while (! result)
    {
      result = tolower (*p1) - tolower (*p2);
      if (*p1++ == '\0')
	break;
      p2 ++;
    }

  return result;
}
#ifndef __TARGET_SCL__
strong_alias (stricmp, strcasecmp)
#endif
