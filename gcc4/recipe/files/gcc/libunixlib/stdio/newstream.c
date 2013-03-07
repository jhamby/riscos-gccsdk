/* UnixLib low-level stream implementation.
   Copyright 2001-2013 UnixLib Developers.  */

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
#include <internal/unix.h>

/* Invalidate a stream.  */
void
__invalidate (FILE *stream)
{
  PTHREAD_UNSAFE

  FILE *next = stream->next;
  memset((void *) stream, 0, sizeof(FILE));
  stream->next = next;
}

/* Make a new stream.  */
FILE *
__newstream (void)
{
  PTHREAD_UNSAFE

  FILE *stream = __iob_head;
  /* Look for a previously created stream that is now
     finished with.  */
  while (stream && stream->__magic == _IOMAGIC)
    stream = stream->next;

  if (stream == NULL)
    {
      /* None to reuse, so create a new one and place it
         at the head of the list.  */
      stream = (FILE *) malloc (sizeof (FILE));
      if (stream == NULL)
	return NULL;
      stream->next = __iob_head;
      __iob_head = stream;
    }

  __invalidate (stream);
  stream->__magic = _IOMAGIC;

  return stream;
}

FILE *
__stream_init (int fd, FILE *stream)
{
  PTHREAD_UNSAFE

  /* Set the magic word for this FILE struct.  */
  stream->__magic = _IOMAGIC;
  stream->fd = fd;

#ifdef DEBUG
  debug_printf ("-- __stream_init(): fd = %x\n", fd);
#endif

  /* By default a stream is opened with no buffers.  */
  stream->__iuserbuf = stream->__ouserbuf = 1;

  if (ispipe (stream->fd))
    stream->__ispipe = 1;
  else if (isatty (stream->fd)) /* Line buffering for tty.  */
    setvbuf (stream, NULL, _IOLBF, BUFSIZ);
  else
    {
      /* Set input/output buffering for the stream.  */
      setvbuf (stream, NULL, _IOFBF, BUFSIZ);
    }

  return stream;
}

/* Dissect the given mode string into an __io_mode.
   We have an invalid mode description when both __io_mode::__bits::__read and
   __io_mode::__bits::__write are unset.  */
__io_mode
__getmode (const char *mode)
{
  __io_mode m = { .__allbits = 0 };

  if (*mode == 'a')
    m.__bits.__write = m.__bits.__create = m.__bits.__append = 1;
  else if (*mode == 'w')
    m.__bits.__write = m.__bits.__create = m.__bits.__truncate = 1;
  else if (*mode == 'r')
    m.__bits.__read = 1;
  else
    return m;
  ++mode;

  /* Check on '+', '+b', 'b' or 'b+'.  Any other remaining set of mode
     descriptions are silently ignored.  */
  if (*mode == '+' || (*mode == 'b' && mode[1] == '+'))
    m.__bits.__read = m.__bits.__write = 1;
  if (*mode == 'b' || (*mode == '+' && mode[1] == 'b'))
    m.__bits.__binary = 1;

  return m;
}
