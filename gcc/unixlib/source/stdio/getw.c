/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/getw.c,v $
 * $Date: 2000/07/15 14:52:31 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getw.c,v 1.1.1.1 2000/07/15 14:52:31 nick Exp $";
#endif

#include <stdio.h>

#define INTSIZE 4

__STDIOLIB__

int
getw (register FILE * f)
{
  register int i;

  i = getc (f);
  i |= (getc (f) << 8);
#if INTSIZE > 2
  i |= (getc (f) << 16);
  i |= (getc (f) << 24);
#if INTSIZE > 4
  i |= (getc (f) << 32);
  i |= (getc (f) << 40);
  i |= (getc (f) << 48);
  i |= (getc (f) << 56);
#endif
#endif
  return (ferror (f) ? -1 : i);
}
