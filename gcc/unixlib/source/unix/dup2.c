/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/dup2,v $
 * $Date: 1997/10/19 21:50:52 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: dup2,v 1.7 1997/10/19 21:50:52 unixlib Exp $";
#endif

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <unixlib/fd.h>

int
dup2 (int fd1, int fd2)
{
  int save;

  if ((unsigned int) fd2 >= MAXFD)
    return __set_errno (EINVAL);

  if (fcntl (fd1, F_GETFL) < 0)
    return -1;

  if (fd1 == fd2)
    return fd2;

  /* Close the new file descriptor, if it is open.  */
  save = errno;
  close (fd2);
  (void) __set_errno (save);

  /* Duplicate the old file descriptor.  */
  return fcntl (fd1, F_DUPFD, fd2);
}
