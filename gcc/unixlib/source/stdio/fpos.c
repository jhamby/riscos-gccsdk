/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/fpos,v $
 * $Date: 1999/09/21 10:39:16 $
 * $Revision: 1.9 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fpos,v 1.9 1999/09/21 10:39:16 admin Exp $";
#endif

/* #define DEBUG */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#ifdef DEBUG
#include <sys/os.h>
#endif

__STDIOLIB__

int
fgetpos (FILE * stream, fpos_t * pos)
{
  if (!__validfp (stream) || pos == NULL)
    {
      errno = EINVAL;
      return -1;
    }
  *pos = ftell (stream);
  if (*pos < 0L)
    return -1;

  return 0;
}

int
fsetpos (FILE * stream, const fpos_t * pos)
{
  if (pos == NULL)
    {
      errno = EINVAL;
      return EOF;
    }
  return fseek (stream, *pos, SEEK_SET);
}

int
fseek (FILE * stream, long offset, int w)
{
  long current, result;

  if (!__validfp (stream))
    {
      errno = EINVAL;
      return EOF;
    }

  /* Write out any pending data.  */
  if (stream->__mode.__write && __flsbuf (EOF, stream) == EOF)
    return EOF;

  /* Calculate the current position after file positioning.  */
  current = ftell (stream);
  if (stream->__error)
    return EOF;


  result = 0;
  switch (w)
    {
    case SEEK_SET:
      result = lseek (fileno (stream), offset, SEEK_SET);
      break;
    case SEEK_CUR:
      result = lseek (fileno (stream), current + offset, SEEK_SET);
      break;
    case SEEK_END:
      result = lseek (fileno (stream), offset, SEEK_END);
      break;
    default:
      errno = EINVAL;
      return EOF;
    }
  if (result < 0)
    {
      stream->__error = 1;
      return -1;
    }

  stream->__offset = (fpos_t) result;

#ifdef DEBUG
  os_print ("fseek("); os_prdec (stream->fd);
  os_print ("): newpos="); os_prdec (stream->__offset);
  os_print ("\r\n");
#endif

  /* Set the input counter to zero so we will read in
     fresh data upon the next read.  */
  stream->i_cnt = 0;
  /* Discard the pushed back character.  */
  stream->__pushedback = 0;
  /* Since we are moving the file position, we are no longer at EOF.  */
  stream->__eof = 0;

  return 0;
}

long
ftell (FILE *stream)
{
  if (!__validfp (stream))
    {
      errno = EINVAL;
      return -1;
    }

  /* It can be difficult to determine the correct file position
     if the file is opened for read/write because we are using
     a separate buffer for read and write operations.  We could
     add an extra field to the FILE structure that would be
     set if the last operation was a write, then we could check
     the appropriate fields.  Instead we will take the easy option
     and flush the write buffers.  */
  if (stream->__mode.__read && stream->__mode.__write)
    __flsbuf (EOF, stream);

#ifdef DEBUG
  os_print ("ftell("); os_prdec (stream->fd); os_print ("): ");
#endif

  if (stream->i_base)
    {
#ifdef DEBUG
      os_print ("offset="); os_prdec (stream->__offset);
      os_print (", i_cnt="); os_prdec (stream->i_cnt);
      os_print (", pushed_back="); os_prdec (stream->__pushedback);
      os_print ("\r\n");
#endif
      return (stream->__offset - ( (stream->__pushedback)
				 ? (long) stream->__pushedi_cnt + 1
				 : (long) stream->i_cnt) );
    }

  if (stream->o_base)
    {
#ifdef DEBUG
      os_print ("offset="); os_prdec (stream->__offset);
      os_print (", o_ptr-o_base="); os_prdec (stream->o_ptr-stream->o_base);
      os_print ("\r\n");
#endif
      return (stream->__offset + (long) (stream->o_ptr - stream->o_base));
    }
  return stream->__offset - stream->__pushedback;
  /* NWC 1997/05/26 */
}

void
rewind (FILE *stream)
{
  /* Clear any file errors before and after the seek operation.  */
  clearerr (stream);
  fseek (stream, 0L, SEEK_SET);
  clearerr (stream);
}
