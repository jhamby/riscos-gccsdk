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
bind (int s, const struct sockaddr *name, socklen_t namelen)
{
  if (__socket_valid (s) == -1)
    return -1;

/*  printf("U! bind: fd=%d, realsocket=%d\n",s,(__u->file+s)->r[0]); */
  return _bind ((int)__u->fd[s].handle, name, namelen);
}
