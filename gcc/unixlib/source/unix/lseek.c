/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/lseek.c,v $
 * $Date: 2000/07/15 14:52:44 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: lseek.c,v 1.1.1.1 2000/07/15 14:52:44 nick Exp $";
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
