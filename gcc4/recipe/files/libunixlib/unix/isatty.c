/* isatty ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>
#include <unixlib/dev.h>
#include <unixlib/unix.h>
#include <unixlib/fd.h>
#include <pthread.h>

/* Return 1 if fd is a tty.  */
int
isatty (int fd)
{
  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  return getfd (fd)->devicehandle->type == DEV_TTY;
}
