/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/ioctl.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ioctl.c,v 1.2.2.2 2001/09/04 16:32:04 admin Exp $";
#endif

#include <errno.h>

#include <sys/ioctl.h>
#include <unixlib/unix.h>
#include <unixlib/dev.h>

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
