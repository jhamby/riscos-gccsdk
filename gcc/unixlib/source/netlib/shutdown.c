/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/shutdown,v $
 * $Date: 1997/10/09 20:00:23 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 * (c) Copyright 1997 Nicholas Clark
 ***************************************************************************/

#ifdef EMBED_RCSIS
static const char rcs_id[] = "$Id: shutdown,v 1.4 1997/10/09 20:00:23 unixlib Exp $";
#endif

#include <sys/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>

int
shutdown (int socket, int how)
{
  if (__socket_valid (socket) == -1)
    return -1;

  return _shutdown ((int)__u->fd[socket].handle, how);
}
