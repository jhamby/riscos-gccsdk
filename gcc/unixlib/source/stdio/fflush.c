/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/fflush,v $
 * $Date: 1997/10/09 20:00:33 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fflush,v 1.7 1997/10/09 20:00:33 unixlib Exp $";
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
