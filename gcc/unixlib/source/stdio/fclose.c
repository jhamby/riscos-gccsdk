/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/fclose,v $
 * $Date: 1999/09/21 10:39:16 $
 * $Revision: 1.9 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fclose,v 1.9 1999/09/21 10:39:16 admin Exp $";
#endif

/* #define DEBUG */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#ifdef DEBUG
#include <sys/os.h>
#endif

#include <fcntl.h>

__STDIOLIB__

int
fclose (FILE * stream)
{
  int status;

  if (stream == NULL)
    {
      FILE *f;

      /* Close all streams.  */
      f = __iob_head;
      while (f)
        {
          if (__validfp (f))
            fclose (f);
          f = f->next;
        }
      return 0;
    }

  if (!__validfp (stream))
    {
      (void) __set_errno (EINVAL);
      return EOF;
    }

#ifdef DEBUG
  os_print ("fclose("); os_prdec (stream->fd); os_print ("): ");
#endif

  /* Only flush writable streams.  */
  if (stream->__mode.__write && __flsbuf (EOF, stream) == EOF)
    {
#ifdef DEBUG
      os_print ("EOF\r\n");
#endif
      return EOF;
    }

  /* Free the buffer's storage.  */
  if (!stream->__iuserbuf)
    free (stream->i_base);
  if (!stream->__ouserbuf)
    free (stream->o_base);

  /* Close the file descriptor.  */
  status = 0;
  status = close (stream->fd);

  /* Invalidate the stream, ready for re-use.  */
  __invalidate (stream);

  return (status < 0) ? EOF : 0;
}
