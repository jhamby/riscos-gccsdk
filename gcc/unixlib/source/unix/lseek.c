/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/lseek.c,v $
 * $Date: 2005/03/04 20:59:06 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <errno.h>
#include <unistd.h>

#include <unixlib/dev.h>
#include <sys/types.h>
#include <unixlib/unix.h>

#include <unixlib/fd.h>
#include <pthread.h>

off_t
lseek (int fd, off_t offset, int whence)
{
  struct __unixlib_fd *file_desc;

  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = getfd (fd);

  /* The validity of whence is check by the device specific operation.  */

  return dev_funcall (file_desc->devicehandle->type, lseek,
		    (file_desc, offset, whence));
}

off_t
lseek64 (int fd, off_t offset, int whence)
{
  return lseek(fd, offset, whence);
}
