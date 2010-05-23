/* lldiv ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <stdlib.h>

lldiv_t
lldiv (long long numer, long long denom)
{
  lldiv_t r;
  long long i;

  r.quot = i = numer / denom;
  r.rem = numer - (i * denom);

  return r;
}
