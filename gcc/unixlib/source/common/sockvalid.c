/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/common/c/sockvalid,v $
 * $Date: 1997/10/09 20:00:00 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sockvalid,v 1.4 1997/10/09 20:00:00 unixlib Exp $";
#endif

#include <errno.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <sys/dev.h>
#include <sys/unix.h>

/* Return 0 if 'socket' is both a valid file descriptor
   and refers to a socket device.  Return -1 if not.  */
int __socket_valid (int socket)
{
  if (BADF(socket))
    {
      errno = EBADF;
      return -1;
    }

  if (__u->fd[socket].device != DEV_SOCKET)
    {
      errno = ENOTSOCK;
      return -1;
    }
  return 0;
}
