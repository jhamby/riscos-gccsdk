/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/common/sockvalid.c,v $
 * $Date: 2000/07/15 14:52:18 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sockvalid.c,v 1.1.1.1 2000/07/15 14:52:18 nick Exp $";
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
