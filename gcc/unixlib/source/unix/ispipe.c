/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/ispipe,v $
 * $Date: 1997/10/19 21:50:56 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ispipe,v 1.7 1997/10/19 21:50:56 unixlib Exp $";
#endif

#include <errno.h>
#include <unistd.h>
#include <sys/dev.h>
#include <sys/unix.h>
#include <unixlib/fd.h>

/* Return 1 if fd is a pipe.  */
int
ispipe (int fd)
{
  if (BADF (fd))
    return __set_errno (EBADF);

  return __u->fd[fd].device == DEV_PIPE;
}
