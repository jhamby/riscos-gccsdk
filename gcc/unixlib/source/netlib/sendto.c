/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/sendto,v $
 * $Date: 1997/10/09 20:00:22 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sendto,v 1.6 1997/10/09 20:00:22 unixlib Exp $";
#endif

#include <sys/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>

int
sendto (int s, const void *msg, int len, int flags,
	const struct sockaddr *to, int tolen)
{
/*  printf("U! sendto: fd=%d, realsocket=%d\n",s,(__u->file+s)->r[0]); */
  if (__socket_valid (s) == -1)
    return -1;

  return _sendto ((int)__u->fd[s].handle, msg, len, flags, to, tolen);
}
