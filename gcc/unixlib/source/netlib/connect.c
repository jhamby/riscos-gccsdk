/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/connect,v $
 * $Date: 1997/10/09 20:00:17 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: connect,v 1.6 1997/10/09 20:00:17 unixlib Exp $";
#endif

#include <sys/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>

int
connect (int s, const struct sockaddr *name, int namelen)
{
  if (__socket_valid (s) == -1)
    return -1;

  /* printf("U! connect: fd=%d, realsocket=%d\n",s,(__u->file+s)->r[0]); */
  return _connect ((int)__u->fd[s].handle, name, namelen);
}
