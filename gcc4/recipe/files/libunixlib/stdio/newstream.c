/* UnixLib low-level stream implementation.
   Copyright 2001-2007 UnixLib Developers.  */

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <unixlib/unix.h>

/* Invalidate a stream.  */
void
__invalidate (FILE *stream)
{
  FILE *next;

  PTHREAD_UNSAFE

  next = stream->next;
  memset((void *) stream, 0, sizeof(FILE));
  stream->next = next;
}

/* Make a new stream.  */
FILE *
__newstream (void)
{
  FILE *stream;

  PTHREAD_UNSAFE

  stream = __iob_head;
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
  stream->__offset = (fpos_t)0;

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
      if (stream->__mode.__bits.__append)
	stream->__offset = (fpos_t) lseek (fd, 0L, SEEK_END);
      else
	stream->__offset = (fpos_t) lseek (fd, 0L, SEEK_CUR);

      /* Set input/output buffering for the stream.  */
      setvbuf (stream, NULL, _IOFBF, BUFSIZ);
    }

  return stream;
}

/* Dissect the given mode string into an __io_mode.  */
__io_mode
__getmode (const char *mode)
{
  __io_mode m;

  m.__allbits = 0;

  if (*mode == 'a')
    m.__bits.__write = m.__bits.__create = m.__bits.__append = 1;
  else if (*mode == 'w')
    m.__bits.__write = m.__bits.__create = m.__bits.__truncate = 1;
  else if (*mode == 'r')
    m.__bits.__read = 1;
  else
    return m;

  ++mode;
  if (*mode == '+' || (*mode && mode[1] == '+'))
    m.__bits.__read = m.__bits.__write = 1;
  if (*mode == 'b' || (*mode && mode[1] == 'b'))
    m.__bits.__binary = 1;

  return m;
}
