/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/div.c,v $
 * $Date: 2001/09/14 14:01:17 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: div.c,v 1.2.2.1 2001/09/14 14:01:17 admin Exp $";
#endif

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
