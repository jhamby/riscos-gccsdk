/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdlib/div.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

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

#ifdef __GNUC__
lldiv_t
lldiv (long long numer, long long denom)
{
  lldiv_t r;
  long long i;

  r.quot = i = numer / denom;
  r.rem = numer - (i * denom);

  return r;
}
#endif
