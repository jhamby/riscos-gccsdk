/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/getsockopt,v $
 * $Date: 1997/10/09 20:00:18 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getsockopt,v 1.6 1997/10/09 20:00:18 unixlib Exp $";
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
