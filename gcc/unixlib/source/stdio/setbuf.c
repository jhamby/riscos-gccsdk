/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/stdio/c/setbuf,v $
 * $Date: 1997/09/19 21:43:43 $
 * $Revision: 1.9 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: setbuf,v 1.9 1997/09/19 21:43:43 unixlib Exp $";
#endif

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

__STDIOLIB__


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
setbuf (register FILE * f, char *buf)
{
  setvbuf (f, buf, (buf) ? _IOFBF : _IONBF, BUFSIZ);
}

static int do_buffer (unsigned char **base, unsigned char **ptr, int *cnt,
		      unsigned int *bufsize, unsigned int *userbuf,
		      char *buf, int flag, int newbufsize)
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

  if (!__validfp (stream))
    {
      (void) __set_errno (EINVAL);
      return EOF;
    }

  if (!stream->__mode.__read && !stream->__mode.__write)
    return EOF;

  /* ANSI says that setvbuf can only be called before any I/O is
     done, but we will allow it to replace at any time. Though
     any old contents of the buffer will be lost.  */
  if (stream->__mode.__read)
    {
      userbuf = stream->__iuserbuf;

      if (do_buffer (&stream->i_base, &stream->i_ptr, &stream->i_cnt,
		     &stream->__bufsize, &userbuf, buf, flag, bufsiz) == EOF)
	return EOF;

      stream->__iuserbuf = userbuf;
    }
  if (stream->__mode.__write)
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
