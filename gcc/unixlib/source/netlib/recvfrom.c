/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/recvfrom.c,v $
 * $Date: 2000/07/15 14:52:25 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: recvfrom.c,v 1.1.1.1 2000/07/15 14:52:25 nick Exp $";
#endif

#include <sys/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>

int
recvfrom (int s, void *msg, int len, int flags,
	  struct sockaddr *from, int *fromlen)
{
  if (__socket_valid (s) == -1)
    return -1;

  return _recvfrom ((int)__u->fd[s].handle, msg, len, flags, from, fromlen);
}
