/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/recvfrom.c,v $
 * $Date: 2005/03/04 20:59:05 $
 * $Revision: 1.7 $
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
recvfrom (int s, void *msg, size_t len, int flags,
	  struct sockaddr *from, socklen_t *fromlen)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (__socket_valid (s) == -1)
    return -1;

  return _recvfrom ((int)(getfd (s)->devicehandle->handle), msg, len, flags, from, fromlen);
}
