/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/fchown,v $
 * $Date: 1997/10/19 21:50:53 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fchown,v 1.1 1997/10/19 21:50:53 unixlib Exp $";
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
