/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/getsocknam,v $
 * $Date: 1997/10/09 20:00:18 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getsocknam,v 1.6 1997/10/09 20:00:18 unixlib Exp $";
#endif

#include <sys/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>

int
getsockname (int s, struct sockaddr *name, int *namelen)
{
  if (__socket_valid (s) == -1)
    return -1;

  return _getsockname ((int)__u->fd[s].handle, name, namelen);
}
