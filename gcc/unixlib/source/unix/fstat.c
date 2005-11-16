/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/fstat.c,v $
 * $Date: 2005/04/14 15:17:23 $
 * $Revision: 1.11 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>

#include <unixlib/dev.h>
#include <unixlib/os.h>
#include <sys/stat.h>
#include <unixlib/unix.h>

#include <unixlib/local.h>
#include <internal/swiparams.h>
#include <pthread.h>

int
fstat (int fd, struct stat *buf)
{
  struct __unixlib_fd *file_desc;

  PTHREAD_UNSAFE

  if (buf == NULL)
    return __set_errno (EINVAL);

  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = getfd (fd);

  buf->st_dev = file_desc->devicehandle->type;

  /* Perform the device specific open operation.  */
  return dev_funcall (file_desc->devicehandle->type, fstat, (fd, buf));
}

int
fstat64 (int fd, struct stat *buf)
{
  return fstat(fd, buf);
}
