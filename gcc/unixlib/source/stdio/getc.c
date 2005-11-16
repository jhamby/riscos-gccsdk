/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/getc.c,v $
 * $Date: 2003/04/13 16:21:02 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <pthread.h>

__STDIOLIB__

int
fgetc (FILE * stream)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream) || !stream->__mode.__read)
    {
      errno = EINVAL;
      return -1;
    }
  return getc_unlocked (stream);
}

#undef getc
#undef getchar

int
getc (FILE * stream)
{
  return fgetc (stream);
}

int
getchar (void)
{
  return fgetc (stdin);
}
