/* UnixLib fgetpos(), fsetpos(), fseek(), fseeko(), ftell(), ftello(),
   rewind() implementation.
   Copyright 2001-2006 UnixLib Developers.  */

/* #define DEBUG */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#ifdef DEBUG
#include <unixlib/os.h>
#endif

__STDIOLIB__

int
fgetpos (FILE * stream, fpos_t * pos)
{
  PTHREAD_UNSAFE

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

  PTHREAD_UNSAFE

  if (!__validfp (stream))
    {
      errno = EINVAL;
      return EOF;
    }

  /* Write out any pending data.  */
  if (stream->__mode.__bits.__write && __flsbuf (EOF, stream) == EOF)
    return EOF;

  /* Calculate the current position after file positioning.  */
  current = ftell (stream);
  if (stream->__error)
    return EOF;

  result = 0;
  switch (w)
    {
    case SEEK_SET:
      if (offset < 0)
	{
	  /* A negative file offset makes no sense, but it does not
	     need to put the stream into error.  */
	  errno = EINVAL;
	  return EOF;
	}
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
  __os_print ("fseek("); __os_prdec (stream->fd);
  __os_print ("): newpos="); __os_prdec (stream->__offset);
  __os_nl ();
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

int
fseeko (FILE * stream, __off_t offset, int w)
{
  return fseek(stream, offset, w);
}

long
ftell (FILE *stream)
{
  PTHREAD_UNSAFE

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
  if (stream->__mode.__bits.__read && stream->__mode.__bits.__write)
    __flsbuf (EOF, stream);

#ifdef DEBUG
  __os_print ("ftell("); __os_prdec (stream->fd); __os_print ("): ");
#endif

  if (stream->i_base)
    {
#ifdef DEBUG
      __os_print ("offset="); __os_prdec (stream->__offset);
      __os_print (", i_cnt="); __os_prdec (stream->i_cnt);
      __os_print (", pushed_back="); __os_prdec (stream->__pushedback);
      __os_nl ();
#endif
      return (stream->__offset - ( (stream->__pushedback)
				 ? (long) stream->__pushedi_cnt + 1
				 : (long) stream->i_cnt) );
    }

  if (stream->o_base)
    {
#ifdef DEBUG
      __os_print ("offset="); __os_prdec (stream->__offset);
      __os_print (", o_ptr-o_base="); __os_prdec (stream->o_ptr-stream->o_base);
      __os_nl ();
#endif
      return (stream->__offset + (long) (stream->o_ptr - stream->o_base));
    }
  return stream->__offset - stream->__pushedback;
  /* NWC 1997/05/26 */
}

__off_t
ftello (FILE *stream)
{
  return ftell(stream);
}

void
rewind (FILE *stream)
{
  PTHREAD_UNSAFE

  /* Clear any file errors before and after the seek operation.  */
  clearerr (stream);
  fseek (stream, 0L, SEEK_SET);
  clearerr (stream);
}
