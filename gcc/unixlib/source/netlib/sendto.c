/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/sendto.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sendto.c,v 1.2.2.1 2001/09/04 16:32:04 admin Exp $";
#endif

#include <unixlib/unix.h>
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
