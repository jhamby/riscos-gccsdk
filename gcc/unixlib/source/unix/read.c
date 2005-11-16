/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/read.c,v $
 * $Date: 2005/03/04 20:59:06 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <unixlib/dev.h>
#include <sys/types.h>
#include <unixlib/unix.h>

#include <unixlib/fd.h>
#include <pthread.h>

ssize_t
read (int fd, void *buf, size_t nbytes)
{
  struct __unixlib_fd *file_desc;

  PTHREAD_UNSAFE_CANCELLATION

  if (nbytes == 0)
    return 0;
  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = getfd (fd);

  /* Confirm that the file is open for reading.  */
  if ((file_desc->fflag & O_ACCMODE) == O_WRONLY)
    return __set_errno (EBADF);

  /* Increment the number of times we have had to read from a device.  */
  __u->usage.ru_inblock++;

  /* Perform the device specific read operation.  */
  return dev_funcall (file_desc->devicehandle->type, read,
		    (file_desc, buf, nbytes));
}
