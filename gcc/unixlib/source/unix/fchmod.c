/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/fchmod.c,v $ $/usr/local/cvsroot/unixlib/source/unix/c/chmod,v $
 * $Date: 2000/07/15 14:52:43 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fchmod.c,v 1.1.1.1 2000/07/15 14:52:43 nick Exp $";
#endif

#include <errno.h>
#include <limits.h>
#include <unistd.h>

#include <sys/dev.h>
#include <sys/os.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/unix.h>

#include <unixlib/local.h>

int fchmod (int fd, mode_t mode)
{
  struct __unixlib_fd *file_desc;
  _kernel_oserror *err;
  int regs[10];
  char name[_POSIX_NAME_MAX];

  if (BADF (fd))
    return __set_errno (EBADF);

  /* Get the file name associated with the file descriptor.  */
  file_desc = &__u->fd[fd];

  if (file_desc->device != DEV_RISCOS)
    return __set_errno (EINVAL);

  if (__fd_to_name ((int) file_desc->handle, name, sizeof (name)) == NULL)
    return __set_errno (EBADF);

  /* Set the file access permission bits.  */
  regs[5] = __set_protection (mode);
  err = os_file (4, name, regs);
  if (err)
    {
      __seterr (err);
      return __set_errno (EPERM);
    }

  return 0;
}
