/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/recv.c,v $
 * $Date: 2000/07/15 14:52:25 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: recv.c,v 1.1.1.1 2000/07/15 14:52:25 nick Exp $";
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
