/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id$";
#endif

#include <errno.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <unixlib/dev.h>
#include <unixlib/unix.h>
#include <pthread.h>

/* Return 0 if 'socket' is both a valid file descriptor
   and refers to a socket device.  Return -1 if not.  */
int __socket_valid (int socket)
{
  PTHREAD_UNSAFE

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
