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
setsockopt (int s, int level, int optname, const void *optval, socklen_t optlen)
{
  if (__socket_valid (s) == -1)
    return -1;

  return _setsockopt ((int)__u->fd[s].handle, level, optname, optval, optlen);
}
