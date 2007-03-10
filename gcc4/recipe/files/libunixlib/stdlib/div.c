/* div (), ldiv (), lldiv ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <stdlib.h>

div_t
div (int numer, int denom)
{
  div_t r;
  int i;

  r.quot = i = numer / denom;
  r.rem = numer - (i * denom);

  return r;
}

ldiv_t
ldiv (long numer, long denom)
{
  ldiv_t r;
  long i;

  r.quot = i = numer / denom;
  r.rem = numer - (i * denom);

  return r;
}

lldiv_t
lldiv (long long numer, long long denom)
{
  lldiv_t r;
  long long i;

  r.quot = i = numer / denom;
  r.rem = numer - (i * denom);

  return r;
}
