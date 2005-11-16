/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/bind.c,v $
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
bind (int s, const struct sockaddr *name, socklen_t namelen)
{
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

  return _bind ((int)(getfd (s)->devicehandle->handle), name, namelen);
}
