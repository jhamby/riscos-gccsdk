/* Compatibility functions for the libscl.
   Implementation taken from UnixLib.  */

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
