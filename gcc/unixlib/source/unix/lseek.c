/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/lseek.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: lseek.c,v 1.2.2.2 2001/09/04 16:32:04 admin Exp $";
#endif

#include <errno.h>
#include <unistd.h>

#include <unixlib/dev.h>
#include <sys/types.h>
#include <unixlib/unix.h>

#include <unixlib/fd.h>

off_t
lseek (int fd, off_t offset, int whence)
{
  struct __unixlib_fd *file_desc;

  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = &__u->fd[fd];

  /* The validity of whence is check by the device specific operation.  */

  return __funcall ((*(__dev[file_desc->device].lseek)),
		    (file_desc, offset, whence));
}
