/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/puts.c,v $
 * $Date: 2000/07/15 14:52:32 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: puts.c,v 1.1.1.1 2000/07/15 14:52:32 nick Exp $";
#endif

#include <stdio.h>

__STDIOLIB__

int
puts (const char *s)
{
  if (fputs (s, stdout) || putchar ('\n') == EOF)
    return EOF;

  return 0;
}
