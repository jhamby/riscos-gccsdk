/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/fchown.c,v $
 * $Date: 2000/07/15 14:52:43 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fchown.c,v 1.1.1.1 2000/07/15 14:52:43 nick Exp $";
#endif

#include <errno.h>
#include <unistd.h>

#include <sys/dev.h>
#include <sys/types.h>
#include <sys/unix.h>

#include <unixlib/fd.h>

#define IGNORE(x) x = x

int
fchown (int fd, uid_t owner, gid_t group)
{
  struct __unixlib_fd *file_desc;

  if (BADF (fd))
    return __set_errno (EBADF);

  /* Get the file name associated with the file descriptor.  */
  file_desc = &__u->fd[fd];

  if (file_desc->device != DEV_RISCOS)
    return __set_errno (EINVAL);

  IGNORE (owner);
  IGNORE (group);
  /* FIXME Should we say ENOSYS ?  */
  return 0;
}
