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
getsockname (int s, struct sockaddr *name, socklen_t *namelen)
{
  if (__socket_valid (s) == -1)
    return -1;

  return _getsockname ((int)__u->fd[s].handle, name, namelen);
}
