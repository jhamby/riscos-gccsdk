/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/sendto.c,v $
 * $Date: 2002/12/22 18:22:29 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sendto.c,v 1.4 2002/12/22 18:22:29 admin Exp $";
#endif

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

int
sendto (int s, const void *msg, size_t len, int flags,
	const struct sockaddr *to, socklen_t tolen)
{
  PTHREAD_UNSAFE_CANCELLATION

/*  printf("U! sendto: fd=%d, realsocket=%d\n",s,(__u->file+s)->r[0]); */
  if (__socket_valid (s) == -1)
    return -1;

  return _sendto ((int)__u->fd[s].handle, msg, len, flags, to, tolen);
}
