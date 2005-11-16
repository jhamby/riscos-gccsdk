/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/listen.c,v $
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
listen (int s, int backlog)
{
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

/*  printf("U! listen: fd=%d, realsocket=%d\n",s,(__u->file+s)->r[0]); */
  return _listen ((int)(getfd (s)->devicehandle->handle), backlog);
}
