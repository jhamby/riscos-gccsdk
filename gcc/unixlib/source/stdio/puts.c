/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/puts.c,v $
 * $Date: 2003/04/13 16:21:02 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

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
