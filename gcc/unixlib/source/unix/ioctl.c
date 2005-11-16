/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/ioctl.c,v $
 * $Date: 2005/03/04 20:59:06 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

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
