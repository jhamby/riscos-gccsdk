/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/bind.c,v $
 * $Date: 2000/07/15 14:52:24 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: bind.c,v 1.1.1.1 2000/07/15 14:52:24 nick Exp $";
#endif

#include <sys/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>

int
bind (int s, const struct sockaddr *name, int namelen)
{
  if (__socket_valid (s) == -1)
    return -1;

/*  printf("U! bind: fd=%d, realsocket=%d\n",s,(__u->file+s)->r[0]); */
  return _bind ((int)__u->fd[s].handle, name, namelen);
}
