/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/fflush.c,v $
 * $Date: 2000/07/15 14:52:31 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fflush.c,v 1.1.1.1 2000/07/15 14:52:31 nick Exp $";
#endif

#include <stdio.h>
#include <errno.h>

__STDIOLIB__

int
fflush (FILE *stream)
{
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

  if (!__validfp (stream) || !stream->__mode.__write)
    {
      errno = EINVAL;
      return EOF;
    }

  return __flsbuf (EOF, stream);
}
