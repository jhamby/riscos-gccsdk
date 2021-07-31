/* ioctl ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <errno.h>
#include <pthread.h>
#include <sys/ioctl.h>

#include <internal/unix.h>
#include <internal/dev.h>

int
ioctl (int fd, unsigned long request, void *arg)
{
  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  struct __unixlib_fd *file_desc = getfd (fd);

  return dev_funcall (file_desc->devicehandle->type, ioctl,
		      (file_desc, request, arg));
}
