/* UnixLib __reopen() implementation.
   Copyright (c) 2000-2010 UnixLib Developers.  */

#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <internal/unix.h>
#include <internal/dev.h>
#include <internal/fd.h>
#include <internal/local.h>
#include <pthread.h>

int
__reopen (int fd, const char *file, int oflag, ...)
{
  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  int mode;
  if (oflag & O_CREAT)
    {
      va_list ap;
      va_start (ap, oflag);
      mode = va_arg (ap, int);
      va_end (ap);
    }
  else
    mode = 0777;

  /* First close the file.  */
  if (__close (getfd (fd)) == -1)
    return -1;

  /* Now open the file.  */
  return __open_fn (fd, file, oflag, mode);
}
