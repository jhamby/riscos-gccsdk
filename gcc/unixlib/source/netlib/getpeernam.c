/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/getpeernam,v $
 * $Date: 1997/10/09 20:00:17 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getpeernam,v 1.6 1997/10/09 20:00:17 unixlib Exp $";
#endif

#include <sys/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>

int
getpeername (int s, struct sockaddr *name, int *namelen)
{
  if (__socket_valid (s) == -1)
    return -1;

  return _getpeername ((int)__u->fd[s].handle, name, namelen);
}
