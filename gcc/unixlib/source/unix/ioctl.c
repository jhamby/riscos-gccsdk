/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/ioctl.c,v $
 * $Date: 2004/09/09 15:34:52 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ioctl.c,v 1.6 2004/09/09 15:34:52 peter Exp $";
#endif

#include <errno.h>

#include <sys/ioctl.h>
#include <unixlib/unix.h>
#include <unixlib/dev.h>
#include <pthread.h>

int
ioctl (int fd, unsigned long request, void *arg)
{
  struct __unixlib_fd *file_desc;

  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = getfd (fd);

  return dev_funcall (file_desc->devicehandle->type, ioctl,
		    (file_desc, request, arg));
}
