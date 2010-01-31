/* strtoumax ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <inttypes.h>
#include <stdlib.h>

uintmax_t
strtoumax (const char *nptr, char **endptr, int base)
{
  return strtoull (nptr, endptr, base);
}
