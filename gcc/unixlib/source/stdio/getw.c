/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/getw,v $
 * $Date: 1997/10/09 20:00:35 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getw,v 1.4 1997/10/09 20:00:35 unixlib Exp $";
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
