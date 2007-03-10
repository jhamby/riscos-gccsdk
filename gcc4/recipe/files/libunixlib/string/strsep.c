/* strsep ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <string.h>

char *
strsep (char **s, const char *delim)
{
  char *begin, *end;

  begin = *s;
  if (! begin)
    return NULL;

  end = strpbrk (begin, delim);
  if (end)
    {
      *end++ = '\0';
      *s = end;
    }
  else
    *s = NULL;

  return begin;
}
