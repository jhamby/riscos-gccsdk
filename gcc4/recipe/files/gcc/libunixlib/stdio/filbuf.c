/* UnixLib buffer stream handling.
   Copyright 2000-2010 UnixLib Developers.  */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <pthread.h>
#include <internal/unix.h>

/* Check the stream for validity and errors.  */
static int
check_stream (FILE *stream)
{
  if (!__validfp (stream) || !stream->__mode.__bits.__read)
    {
      (void) __set_errno (EINVAL);
      return EOF;
    }

  /* To the best of my knowledge this fuction is only called by getc() and
     fread(). In fread() feof() is never true. */
  if (stream->__eof)
    {
      /* getc() has just decremented i_cnt to -1.
	 If we don't restore it to 0 ftell() returns <off by one (more)>
	 whenever program attempts getc() at feof() */
      stream->i_cnt = 0;
      return EOF;
    }
  if (stream->__error)
    return EOF;

  return 0;
}

/* Fill the input buffer.  */
static int
fill_buffer (FILE *stream)
{
  int count, to_read = 0;
  unsigned char *buffer;
  static char c; /* a 1 character buffer */

  /* String streams cannot be refilled, so set __eof and return.
     I do not think errno should be set here.  */
  if (stream->__string_istream)
    {
      stream->__eof = 1;
      return EOF;
    }

  if (stream->i_base == NULL)
    {
      /* We are unbuffered, so only read one character.  */
      buffer = (unsigned char *)&c;
      to_read = 1;
      /* The statically allocated 1 character buffer is used
	 in scanf().  */
      stream->i_ptr = buffer;
    }
  else
    {
      buffer = stream->i_base;
      to_read = stream->__bufsize;
    }

  count = read (stream->fd, buffer, to_read);
  if (count == -1 || (to_read != 0 && count == 0))
    {
      /* A file error occurred.  */
      if (count == -1)
	stream->__error = 1;
      else
	stream->__eof = 1;

      if (stream->i_base != NULL)
	{
	  stream->i_cnt = 0;
	  stream->i_ptr = stream->i_base + stream->__bufsize;
	}
      return EOF;
    }

  stream->__offset += count;
  return count;
}

/* Return the next character in the input buffer, removing it
   from the buffer. Fill the buffer if it is empty.  */
int
__filbuf (FILE *stream)
{
  int count;

  PTHREAD_UNSAFE

  if (check_stream (stream) == EOF)
    return EOF;

  /* When we have possibly unflushed data in one of our line buffered streams
     and we're reading from a tty attached stream, flush all those streams.  */
  if (__ul_global.fls_lbstm_on_rd && isatty (fileno (stream)) && __flslbbuf ())
    return EOF;

  if (stream->__pushedback)
    {
      /* Return the character pushed back by ungetc. Restore
	 the i_cnt pointer.  */
      stream->__pushedback = 0;
      stream->i_cnt = stream->__pushedi_cnt;
      return stream->__pushedchar;
    }

  count = fill_buffer (stream);
  if (count == EOF)
    return EOF;

  if (stream->i_base == NULL)
    /* No buffer, so return the char we read.  */
    return (stream->__eof || stream->__error) ? EOF : *stream->i_ptr;

  stream->i_cnt = count - 1;
  stream->i_ptr = stream->i_base;
  return *stream->i_ptr++;
}

/* Return the next character in the input buffer, keeping it in
   the input buffer.  If the buffer is empty, then fill it.  */
int
__peek_char (FILE *stream)
{
  int count;

  PTHREAD_UNSAFE

  if (check_stream (stream) == EOF)
    return EOF;

  /* When we have possibly unflushed data in one of our line buffered streams
     and we're reading from a tty attached stream, flush all those streams.  */
  if (__ul_global.fls_lbstm_on_rd && isatty (fileno (stream)) && __flslbbuf ())
    return EOF;

  if (stream->__pushedback)
    return stream->__pushedchar;

  /* If we've still got stuff in the buffer, then return a
     character.  */
  if (stream->i_cnt > 0)
    return *stream->i_ptr;

  count = fill_buffer (stream);
  if (count == EOF)
    return EOF;

  if (stream->i_base == NULL)
    /* No buffer, so return the char we read.  */
    return (stream->__eof || stream->__error) ? EOF : *stream->i_ptr;

  stream->i_cnt = count;
  stream->i_ptr = stream->i_base;
  return *stream->i_ptr;
}
