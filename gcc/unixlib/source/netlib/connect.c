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
connect (int s, const struct sockaddr *name, int namelen)
{
  if (__socket_valid (s) == -1)
    return -1;

  /* printf("U! connect: fd=%d, realsocket=%d\n", s, __u->fd[s].handle); */
  return _connect ((int)__u->fd[s].handle, name, namelen);
}
