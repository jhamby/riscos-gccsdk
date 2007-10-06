/* strchr ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <stdlib.h>
#include <string.h>

char *
strchr (const char *s, int c_)
{
  int i;
  char c = (char) c_;

  while ((i = *s) && (i != c))
    s++;

  if (i == c)
    return (char *) s;

  return NULL;
}
