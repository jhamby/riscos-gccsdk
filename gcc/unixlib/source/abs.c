/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/abs,v $
 * $Date: 1997/10/09 19:59:37 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: abs,v 1.4 1997/10/09 19:59:37 unixlib Exp $";
#endif

#include <stdlib.h>

int
abs (register int x)

{
  return ((x < 0) ? -x : x);
}

long
labs (register long x)

{
  return ((x < 0) ? -x : x);
}
