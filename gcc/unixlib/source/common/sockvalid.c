/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/common/sockvalid.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sockvalid.c,v 1.2.2.1 2001/09/04 16:32:04 admin Exp $";
#endif

#include <errno.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <unixlib/dev.h>
#include <unixlib/unix.h>

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
