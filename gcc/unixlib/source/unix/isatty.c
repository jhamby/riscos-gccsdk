/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/isatty.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: isatty.c,v 1.2.2.1 2001/09/04 16:32:04 admin Exp $";
#endif

#include <errno.h>
#include <unistd.h>
#include <unixlib/dev.h>
#include <unixlib/unix.h>
#include <unixlib/fd.h>

/* Return 1 if fd is a tty.  */
int
isatty (int fd)
{
  if (BADF (fd))
    return __set_errno (EBADF);

  return __u->fd[fd].device == DEV_TTY;
}
