/* ldiv ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <stdlib.h>

ldiv_t
ldiv (long numer, long denom)
{
  ldiv_t r;
  long i;

  r.quot = i = numer / denom;
  r.rem = numer - (i * denom);

  return r;
}


