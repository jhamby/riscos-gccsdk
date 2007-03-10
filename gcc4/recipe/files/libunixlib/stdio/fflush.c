/* UnixLib fflush() implementation.
   Copyright 2000-2006 UnixLib Developers.  */

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
	  if (__validfp (stream) && stream->__mode.__bits.__write)
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

  if (!stream->__mode.__bits.__write)
    return 0;

  return __flsbuf (EOF, stream);
}
