/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id$";
#endif

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>

int
sendto (int s, const void *msg, size_t len, int flags,
	const struct sockaddr *to, socklen_t tolen)
{
/*  printf("U! sendto: fd=%d, realsocket=%d\n",s,(__u->file+s)->r[0]); */
  if (__socket_valid (s) == -1)
    return -1;

  return _sendto ((int)__u->fd[s].handle, msg, len, flags, to, tolen);
}
