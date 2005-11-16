/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/getsockopt.c,v $
 * $Date: 2005/03/04 20:59:05 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: alex $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

int
getsockopt (int s, int level, int optname, void *optval, socklen_t *optlen)
{
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

  return _getsockopt ((int)(getfd (s)->devicehandle->handle), level, optname, optval, optlen);
}
