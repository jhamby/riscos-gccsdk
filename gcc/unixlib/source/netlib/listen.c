/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/listen,v $
 * $Date: 1997/10/09 20:00:20 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: listen,v 1.6 1997/10/09 20:00:20 unixlib Exp $";
#endif

#include <sys/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>

int
listen (int s, int backlog)
{
  if (__socket_valid (s) == -1)
    return -1;

/*  printf("U! listen: fd=%d, realsocket=%d\n",s,(__u->file+s)->r[0]); */
  return _listen ((int)__u->fd[s].handle, backlog);
}
