/* strchr ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <string.h>

char *
strchr (const char *s, int c_)
{
  int i;
  char c = (char) c_;

  while ((i = *s) && (i != c))
    s++;

  if (i == c)
    return ((char *) s);
  else
    return (0);
}
