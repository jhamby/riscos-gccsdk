/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/recvfrom,v $
 * $Date: 1997/10/09 20:00:21 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: recvfrom,v 1.6 1997/10/09 20:00:21 unixlib Exp $";
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
