/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/flsbuf,v $
 * $Date: 1999/09/21 10:39:16 $
 * $Revision: 1.9 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: flsbuf,v 1.9 1999/09/21 10:39:16 admin Exp $";
#endif

/* #define DEBUG */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#ifdef DEBUG
#include <sys/os.h>
#endif

__STDIOLIB__

/* __flsbuf(-1,f) flushes output without adding c */

int
__flsbuf (int c, FILE *stream)
{
  unsigned char *buffer;

  if (!__validfp (stream) || !stream->__mode.__write)
    {
      (void) __set_errno (EINVAL);
      return EOF;
    }

  if (ferror (stream))
    return EOF;

#ifdef DEBUG
  os_print ("__flsbuf("); os_prdec (stream->fd); os_print ("): ");
#endif

  if (stream->__pushedback)
    {
      /* Discard the char pushed back by ungetc.  */
      stream->i_cnt = stream->__pushedi_cnt;
      stream->__pushedback = 0;
    }

  if (stream->o_base != NULL)
    {
      int to_write, written;

      to_write = stream->o_ptr - stream->o_base;
#ifdef DEBUG
      os_print ("to_write="); os_prdec (to_write); os_print ("\r\n");
#endif
      /* Skip write if 0 characters to write. Keeps perl happy, consistent
         with BSD.  */
      written = to_write ? write (stream->fd, stream->o_base, to_write) : 0;
      if (written < to_write)
        {
          stream->__error = 1;
          return EOF;
        }
      stream->__offset += to_write;
      stream->o_cnt = stream->__bufsize;
      stream->o_ptr = stream->o_base;
    }
#ifdef DEBUG
  else
    {
      os_print ("\r\n");
    }
#endif

  if (c == EOF)
    return 0;

  /* Write out the last character.  */
  buffer = (unsigned char *)&c;
  if (write (stream->fd, buffer, 1) < 1)
    {
      stream->__error = 1;
      return EOF;
    }
  stream->__offset += 1;

  return c;
}
