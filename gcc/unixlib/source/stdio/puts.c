/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/puts,v $
 * $Date: 1997/10/09 20:00:36 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: puts,v 1.5 1997/10/09 20:00:36 unixlib Exp $";
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
