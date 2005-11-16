/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/recvmsg.c,v $
 * $Date: 2005/03/04 20:59:05 $
 * $Revision: 1.5 $
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
recvmsg (int s, struct msghdr *msg, int flags)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (__socket_valid (s) == -1)
    return -1;

  return _recvmsg ((int)(getfd (s)->devicehandle->handle), msg, flags);
}
