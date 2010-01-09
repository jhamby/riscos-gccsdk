/* UnixLib fgetpos(), fsetpos(), fseek(), fseeko(), ftell(), ftello(),
   rewind() implementation.
   Copyright 2001-2010 UnixLib Developers.  */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>
#include <internal/unix.h>

/* #define DEBUG */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

int
fgetpos (FILE * stream, fpos_t * pos)
{
  PTHREAD_UNSAFE

  if (!__validfp (stream) || pos == NULL)
    return __set_errno (EINVAL);

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
      (void) __set_errno (EINVAL);
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
      (void) __set_errno (EINVAL);
      return EOF;
    }

  /* Write out any pending data.  */
  if (stream->__mode.__bits.__write && __flsbuf (EOF, stream) == EOF)
    return EOF;

  /* Calculate the current position after file positioning.  */
  current = ftell (stream);
  if (stream->__error)
    return EOF;

  switch (w)
    {
    case SEEK_SET:
      if (offset < 0)
	{
	  /* A negative file offset makes no sense, but it does not
	     need to put the stream into error.  */
	  (void) __set_errno (EINVAL);
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
      (void) __set_errno (EINVAL);
      return EOF;
    }
  if (result < 0)
    {
      stream->__error = 1;
      return -1;
    }

  stream->__offset = (fpos_t) result;

#ifdef DEBUG
  debug_printf ("-- fseek(fd=%d): newpos=%d\n", stream->fd, stream->__offset);
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
    return __set_errno (EINVAL);

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
  debug_printf ("-- ftell(fd=%d): ", stream->fd);
#endif

  if (stream->i_base)
    {
#ifdef DEBUG
      debug_printf ("offset=%d, i_cnt=%d, pushed_back=%d\n",
		    stream->__offset, stream->i_cnt, stream->__pushedback);
#endif
      return stream->__offset - ( (stream->__pushedback)
				? (long) stream->__pushedi_cnt + 1
				: (long) stream->i_cnt);
    }

  if (stream->o_base)
    {
#ifdef DEBUG
      debug_printf ("offset=%d, o_ptr - o_base=%d\n",
		    stream->__offset, stream->o_ptr - stream->o_base);
#endif
      return stream->__offset + (long) (stream->o_ptr - stream->o_base);
    }

#ifdef DEBUG
  debug_printf ("offset=%d, pushed_back=%d\n",
		stream->__offset, stream->__pushedback);
#endif
  return stream->__offset - stream->__pushedback;
}

__off_t
ftello (FILE *stream)
{
  return ftell (stream);
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
