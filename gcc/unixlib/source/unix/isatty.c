/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/isatty.c,v $
 * $Date: 2000/07/15 14:52:44 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: isatty.c,v 1.1.1.1 2000/07/15 14:52:44 nick Exp $";
#endif

#include <errno.h>
#include <unistd.h>
#include <sys/dev.h>
#include <sys/unix.h>
#include <unixlib/fd.h>

/* Return 1 if fd is a tty.  */
int
isatty (int fd)
{
  if (BADF (fd))
    return __set_errno (EBADF);

  return __u->fd[fd].device == DEV_TTY;
}
