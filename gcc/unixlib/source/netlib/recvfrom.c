/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/recvfrom.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: recvfrom.c,v 1.2.2.1 2001/09/04 16:32:04 admin Exp $";
#endif

#include <unixlib/unix.h>
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
