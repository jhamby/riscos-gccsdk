/* UnixLib freopen() implementation.
   Copyright 2001-2008 UnixLib Developers.  */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#include <pthread.h>
#include <internal/unix.h>
#include <internal/local.h>

/* #define DEBUG */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

FILE *
freopen (const char *filename, const char *mode, FILE *stream)
{
  __io_mode m;
  __mode_t file_mode;
  int fd;

  PTHREAD_UNSAFE

  /* Close the file if it is open.  */
  if (!__validfp (stream))
    {
      (void) __set_errno (EINVAL);
      return NULL;
    }

  if (filename == NULL || mode == NULL)
    {
      __set_errno (EINVAL);
      return NULL;
    }

#ifdef DEBUG
  debug_printf ("--freopen(file=%s,mode=%s,fd=%d\n",
		filename, mode, stream->fd);
#endif

  m = __getmode (mode);
  if (! m.__bits.__read && ! m.__bits.__write)
    {
      fclose (stream);
      (void) __set_errno (EINVAL);
      return NULL;
    }


  /* Flush the stream.  */
  if (stream->__mode.__bits.__write)
    __flsbuf (EOF, stream);

  /* Reset the file buffers.  */
  stream->i_ptr = stream->i_base;
  stream->i_cnt = 0;
  stream->o_cnt = stream->__bufsize;
  stream->o_ptr = stream->o_base;
  /* Discard the pushed back character.  */
  stream->__pushedback = 0;
  /* Since we are reopening, we are no longer at EOF.  */
  stream->__eof = 0;

  stream->__mode = m;

  if (m.__bits.__read && m.__bits.__write)
    file_mode = O_RDWR;
  else
    file_mode = m.__bits.__read ? O_RDONLY : O_WRONLY;

  if (m.__bits.__append)
    file_mode |= O_APPEND;
  if (m.__bits.__truncate)
    file_mode |= O_TRUNC;

  if (m.__bits.__create)
    fd = __reopen (stream->fd, filename, file_mode | O_CREAT,
               S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  else
    fd = __reopen (stream->fd, filename, file_mode);

  if (fd < 0)
    {
      fclose (stream);
      return NULL;
    }

  return stream;
}
