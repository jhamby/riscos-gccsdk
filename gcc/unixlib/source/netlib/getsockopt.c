/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/getsockopt.c,v $
 * $Date: 2002/12/13 15:01:59 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getsockopt.c,v 1.4 2002/12/13 15:01:59 admin Exp $";
#endif

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

int
getsockopt (int s, int level, int optname, void *optval, socklen_t *optlen)
{
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

  return _getsockopt ((int)__u->fd[s].handle, level, optname, optval, optlen);
}
