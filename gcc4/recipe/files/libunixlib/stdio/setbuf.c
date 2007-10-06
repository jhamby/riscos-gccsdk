/* UnixLib setbuffer(), setlinebuf(), setbuf() and setvbuf() implementation.
   Copyright 2000-2007 UnixLib Developers.  */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unixlib/unix.h>

/* If buf is null, this makes stream unbuffered.
   Otherwise it makes stream fully buffered using buf
   as the buffer (of size size).  */
void
setbuffer (FILE * stream, char *buf, size_t size)
{
  setvbuf (stream, buf, (buf) ? _IOFBF : _IONBF, size);
}

/* Make stream be line buffered, and allocate the buffer.  */

void
setlinebuf (FILE * stream)
{
  setvbuf (stream, NULL, _IOLBF, BUFSIZ);
}

void
setbuf (FILE * f, char *buf)
{
  setvbuf (f, buf, (buf) ? _IOFBF : _IONBF, BUFSIZ);
}

static int
do_buffer (unsigned char **base, unsigned char **ptr, int *cnt,
	   size_t *bufsize, unsigned int *userbuf,
	   char *buf, int flag, size_t newbufsize)
{
  if (*base && !*userbuf)
    free (*base);

  *userbuf = 0;
  *cnt = 0;
  if (flag == _IOFBF || flag == _IOLBF)
    {
      if (newbufsize == 0)
	{
	  (void) __set_errno (EINVAL);
	  return EOF;
	}
      *bufsize = newbufsize;
      if (buf != NULL)
	*userbuf = 1;
      else
	buf = malloc (newbufsize);
      *base = (unsigned char *)buf;
      *ptr = *base;
      if (buf == NULL)
	return EOF;
    }
  else if (flag == _IONBF)
    {
      *base = NULL;
      *ptr = NULL;
      *bufsize = 0;
    }
  else
    {
      (void) __set_errno (EINVAL);
      return EOF;
    }

  return 0;
}

int
setvbuf (FILE * stream, char *buf, int flag, size_t bufsiz)
{
  unsigned int userbuf;

  PTHREAD_UNSAFE

  if (!__validfp (stream))
    {
      (void) __set_errno (EINVAL);
      return EOF;
    }

  if (!stream->__mode.__bits.__read && !stream->__mode.__bits.__write)
    return EOF;

  /* ANSI says that setvbuf can only be called before any I/O is
     done, but we will allow it to replace at any time. Though
     any old contents of the buffer will be lost.  */
  if (stream->__mode.__bits.__read)
    {
      userbuf = stream->__iuserbuf;

      if (do_buffer (&stream->i_base, &stream->i_ptr, &stream->i_cnt,
		     &stream->__bufsize, &userbuf, buf, flag, bufsiz) == EOF)
	return EOF;

      stream->__iuserbuf = userbuf;
    }
  if (stream->__mode.__bits.__write)
    {
      userbuf = stream->__ouserbuf;

      if (do_buffer (&stream->o_base, &stream->o_ptr, &stream->o_cnt,
		     &stream->__bufsize, &userbuf, buf, flag, bufsiz) == EOF)
	return EOF;

      stream->__ouserbuf = userbuf;
    }

  stream->__linebuf = (flag == _IOLBF) ? 1 : 0;
  stream->__pushedback = 0;

  return 0;
}
