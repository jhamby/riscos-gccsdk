/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/read.c,v $
 * $Date: 2000/07/15 14:52:44 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: read.c,v 1.1.1.1 2000/07/15 14:52:44 nick Exp $";
#endif

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/syslib.h>
#include <sys/dev.h>
#include <sys/types.h>
#include <sys/unix.h>

#include <unixlib/fd.h>

ssize_t
read (int fd, void *buf, size_t nbytes)
{
  struct __unixlib_fd *file_desc;

  if (nbytes == 0)
    return 0;
  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = &__u->fd[fd];

  /* Confirm that the file is open for reading.  */
  if ((file_desc->fflag & O_ACCMODE) == O_WRONLY)
    return __set_errno (EBADF);

  /* Increment the number of times we have had to read from a device.  */
  __u->usage.ru_inblock++;

  /* Perform the device specific read operation.  */
  return __funcall ((*(__dev[file_desc->device].read)),
		    (file_desc, buf, nbytes));
}
