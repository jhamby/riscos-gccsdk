/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/ioctl.c,v $
 * $Date: 2000/07/15 14:52:44 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ioctl.c,v 1.1.1.1 2000/07/15 14:52:44 nick Exp $";
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
