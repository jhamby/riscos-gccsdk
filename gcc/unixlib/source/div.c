/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/div,v $
 * $Date: 1997/10/09 19:59:37 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: div,v 1.4 1997/10/09 19:59:37 unixlib Exp $";
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
