/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/dup2.c,v $
 * $Date: 2000/07/15 14:52:43 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: dup2.c,v 1.1.1.1 2000/07/15 14:52:43 nick Exp $";
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
