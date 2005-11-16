/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/common/sockvalid.c,v $
 * $Date: 2005/03/04 20:59:05 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

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

  if (getfd (socket)->devicehandle->type != DEV_SOCKET)
    {
      errno = ENOTSOCK;
      return -1;
    }
  return 0;
}
