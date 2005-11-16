/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/getpeernam.c,v $
 * $Date: 2005/04/07 18:34:11 $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Author: nick $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

int
getpeername (int s, __SOCKADDR_ARG name, socklen_t *__restrict namelen)
{
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

  return _getpeername ((int)(getfd (s)->devicehandle->handle), name, namelen);
}
