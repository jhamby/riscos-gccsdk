/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/putw.c,v $
 * $Date: 2000/07/15 14:52:32 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: putw.c,v 1.1.1.1 2000/07/15 14:52:32 nick Exp $";
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
