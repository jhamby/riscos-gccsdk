/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/recvfrom.c,v $
 * $Date: 2002/12/22 18:22:29 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: admin $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: recvfrom.c,v 1.5 2002/12/22 18:22:29 admin Exp $";
#endif

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

  return _recvfrom ((int)__u->fd[s].handle, msg, len, flags, from, fromlen);
}
