/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/shutdown.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 * (c) Copyright 1997 Nicholas Clark
 ***************************************************************************/

#ifdef EMBED_RCSIS
static const char rcs_id[] = "$Id: shutdown.c,v 1.2.2.1 2001/09/04 16:32:04 admin Exp $";
#endif

#include <unixlib/unix.h>
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
