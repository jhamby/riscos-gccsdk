/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/recv.c,v $
 * $Date: 2002/12/22 18:22:29 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: recv.c,v 1.4 2002/12/22 18:22:29 admin Exp $";
#endif

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

int
recv (int s, void *msg, size_t len, int flags)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (__socket_valid (s) == -1)
    return -1;

/*  printf("U! recv: fd=%d, realsocket=%d\n",s,(__u->file+s)->r[0]); */
  return _recv ((int)__u->fd[s].handle, msg, len, flags);
}
