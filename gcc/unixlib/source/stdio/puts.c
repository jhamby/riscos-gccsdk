/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/puts.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: puts.c,v 1.2 2001/01/29 15:10:21 admin Exp $";
#endif

#include <stdio.h>
#include <pthread.h>

__STDIOLIB__

int
puts (const char *s)
{
  PTHREAD_UNSAFE

  if (fputs (s, stdout) || putchar ('\n') == EOF)
    return EOF;

  return 0;
}
