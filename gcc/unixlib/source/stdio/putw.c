/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/putw,v $
 * $Date: 1997/10/09 20:00:36 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: putw,v 1.4 1997/10/09 20:00:36 unixlib Exp $";
#endif

#include <stdio.h>

#define INTSIZE 4

__STDIOLIB__

int
putw (register int i, register FILE * f)
{
  putc (i & 0xff, f);
  putc ((i >> 8) & 0xff, f);
#if INTSIZE > 2
  putc ((i >> 16) & 0xff, f);
  putc ((i >> 24) & 0xff, f);
#if INTSIZE > 4
  putc ((i >> 32) & 0xff, f);
  putc ((i >> 40) & 0xff, f);
  putc ((i >> 48) & 0xff, f);
  putc ((i >> 56) & 0xff, f);
#endif
#endif
  return (ferror (f) ? -1 : i);
}
