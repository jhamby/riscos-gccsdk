/* mblen ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <stdlib.h>

int
mblen (const char *s, size_t n)
{
  if (s == NULL || *s == '\0')
    return 0;
  if (n == 0)
    return -1;
  return 1;
}
