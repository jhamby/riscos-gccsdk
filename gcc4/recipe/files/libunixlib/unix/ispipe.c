/* ispipe ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <unistd.h>

#include <internal/dev.h>
#include <internal/unix.h>
#include <internal/fd.h>
#include <pthread.h>

/* Return 1 if fd is a pipe.  */
int
ispipe (int fd)
{
  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  return getfd (fd)->devicehandle->type == DEV_PIPE;
}
