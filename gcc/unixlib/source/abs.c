/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/abs.c,v $
 * $Date: 2000/07/15 14:52:08 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: abs.c,v 1.1.1.1 2000/07/15 14:52:08 nick Exp $";
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
