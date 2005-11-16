/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/fflush.c,v $
 * $Date: 2004/12/07 22:57:53 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <pthread.h>

__STDIOLIB__

int
fflush (FILE *stream)
{
  PTHREAD_UNSAFE

  if (stream == NULL)
    {
      int lossage = 0;

      /* Flush all streams.  */
      stream = __iob_head;
      while (stream)
        {
	  if (__validfp (stream) && stream->__mode.__write)
	    lossage |= __flsbuf (EOF, stream);
	  stream = stream->next;
	}
      return lossage ? EOF : 0;
    }

  if (!__validfp (stream))
    {
      errno = EINVAL;
      return EOF;
    }

  if (!stream->__mode.__write)
    return 0;

  return __flsbuf (EOF, stream);
}
