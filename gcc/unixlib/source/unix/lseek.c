/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/lseek,v $
 * $Date: 1997/10/19 21:50:57 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: lseek,v 1.7 1997/10/19 21:50:57 unixlib Exp $";
#endif

#include <errno.h>
#include <unistd.h>

#include <sys/dev.h>
#include <sys/syslib.h>
#include <sys/types.h>
#include <sys/unix.h>

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
