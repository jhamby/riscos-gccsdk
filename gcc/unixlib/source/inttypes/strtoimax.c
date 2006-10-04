/* strtoimax ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <inttypes.h>
#include <stdlib.h>

intmax_t
strtoimax (const char * nptr, char **endptr, int base)
{
  return strtoll (nptr, endptr, base);
}
