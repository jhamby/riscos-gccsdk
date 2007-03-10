/* ioctl ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>

#include <sys/ioctl.h>
#include <unixlib/unix.h>
#include <unixlib/dev.h>
#include <pthread.h>

int
ioctl (int fd, unsigned long request, void *arg)
{
  struct __unixlib_fd *file_desc;

  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = getfd (fd);

  return dev_funcall (file_desc->devicehandle->type, ioctl,
		    (file_desc, request, arg));
}
