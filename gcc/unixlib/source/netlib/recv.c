/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/recv,v $
 * $Date: 1997/10/09 20:00:21 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: recv,v 1.6 1997/10/09 20:00:21 unixlib Exp $";
#endif

#include <sys/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>

int
recv (int s, void *msg, int len, int flags)
{
  if (__socket_valid (s) == -1)
    return -1;

/*  printf("U! recv: fd=%d, realsocket=%d\n",s,(__u->file+s)->r[0]); */
  return _recv ((int)__u->fd[s].handle, msg, len, flags);
}
