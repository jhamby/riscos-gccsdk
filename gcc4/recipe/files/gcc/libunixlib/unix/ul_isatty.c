/* isatty ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>

#include <internal/dev.h>
#include <internal/unix.h>
#include <internal/fd.h>
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
