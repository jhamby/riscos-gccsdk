/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/ioctl,v $
 * $Date: 1997/12/17 22:27:09 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ioctl,v 1.7 1997/12/17 22:27:09 unixlib Exp $";
#endif

#include <errno.h>

#include <sys/ioctl.h>
#include <sys/syslib.h>
#include <sys/unix.h>
#include <sys/dev.h>

int
ioctl (int fd, int request, void *arg)
{
  struct __unixlib_fd *file_desc;

  if (!arg)
    return __set_errno (EINVAL);

  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = &__u->fd[fd];

  return __funcall ((*__dev[file_desc->device].ioctl),
		    (file_desc, request, arg));
}
