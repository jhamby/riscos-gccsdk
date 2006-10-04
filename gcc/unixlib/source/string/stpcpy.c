/* stpcpy ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <string.h>

char *
stpcpy (char *s, const char *s2)
{
  while ((*s++ = *s2++))
    ;

  return s - 1;
}

