/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/div.c,v $
 * $Date: 2000/07/15 14:52:08 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: div.c,v 1.1.1.1 2000/07/15 14:52:08 nick Exp $";
#endif

#include <stdlib.h>

div_t
div (register int numer, register int denom)

{
  register div_t r;
  register int i;

  r.quot = i = numer / denom;
  r.rem = numer - (i * denom);

  return (r);
}

ldiv_t
ldiv (register long numer, register long denom)

{
  register ldiv_t r;
  register long i;

  r.quot = i = numer / denom;
  r.rem = numer - (i * denom);

  return (r);
}
