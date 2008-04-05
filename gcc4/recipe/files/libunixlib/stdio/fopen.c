/* UnixLib fopen()/fdopen() implementation.
   Copyright 2001-2008 UnixLib Developers.  */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#include <internal/unix.h>
#include <pthread.h>

/* #define DEBUG */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

FILE *
fopen (const char *filename, const char *mode)
{
  FILE *stream;
  __io_mode m;
  __mode_t file_mode;
  int fd;

  PTHREAD_UNSAFE

  if (filename == NULL || mode == NULL)
    {
      __set_errno (EINVAL);
      return NULL;
    }

#ifdef DEBUG
  debug_printf ("-- fopen(fname=%s, mode=%s): ", filename, mode);
#endif

  m = __getmode (mode);
  if (! m.__bits.__read && ! m.__bits.__write)
    {
      (void) __set_errno (EINVAL);
      return NULL;
    }

  /* Create new file stream.  */
  stream = __newstream ();
  if (stream == NULL)
    return NULL;

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
    fd = open (filename, file_mode | O_CREAT,
	       S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  else
    fd = open (filename, file_mode);

#ifdef DEBUG
  debug_printf ("fd=%d\n", fd);
#endif

  /* When fd is not valid, we can just quit and leave the stream
     invalidated.  This stream will be picked up and reused the next time
     __newstream is called.  */
  if (fd < 0)
    return NULL;

  return __stream_init (fd, stream);
}

FILE *
fdopen (int fd, const char *mode)
{
  FILE *stream;
  __io_mode m;
  int dflags;

#ifdef DEBUG
  debug_printf ("-- fdopen (fd=%d,mode=%s)\n", fd, mode);
#endif
  m = __getmode (mode);
  if (! m.__bits.__read && ! m.__bits.__write)
    return NULL;

  /* Verify the FD is valid and allows the access 'mode' specifies.  */
  dflags = getfd (fd)->fflag; /* dflags = fcntl (fd, F_GETFL); */
  if (dflags == -1)
    return NULL;

  /* Check the access mode.  */
  if ((dflags & O_ACCMODE) == O_RDONLY && ! m.__bits.__read)
    {
      __set_errno (EBADF);
      return NULL;
    }
  if ((dflags & O_ACCMODE) == O_WRONLY && ! m.__bits.__write)
    {
      __set_errno (EBADF);
      return NULL;
    }

  stream = __newstream ();
  if (stream == NULL)
    return NULL;

  stream->__mode = m;

  return __stream_init (fd, stream);
}
