/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/shutdown.c,v $
 * $Date: 2000/07/15 14:52:26 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 * (c) Copyright 1997 Nicholas Clark
 ***************************************************************************/

#ifdef EMBED_RCSIS
static const char rcs_id[] = "$Id: shutdown.c,v 1.1.1.1 2000/07/15 14:52:26 nick Exp $";
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
