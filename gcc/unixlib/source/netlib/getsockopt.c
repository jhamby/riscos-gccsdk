/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/getsockopt.c,v $
 * $Date: 2000/07/15 14:52:24 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getsockopt.c,v 1.1.1.1 2000/07/15 14:52:24 nick Exp $";
#endif

#include <sys/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>

int
getsockopt (int s, int level, int optname, void *optval, int *optlen)
{
  if (__socket_valid (s) == -1)
    return -1;

  return _getsockopt ((int)__u->fd[s].handle, level, optname, optval, optlen);
}
