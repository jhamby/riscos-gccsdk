/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/shutdown.c,v $
 * $Date: 2002/02/14 15:56:36 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 * (c) Copyright 1997 Nicholas Clark
 ***************************************************************************/

#ifdef EMBED_RCSIS
static const char rcs_id[] = "$Id: shutdown.c,v 1.3 2002/02/14 15:56:36 admin Exp $";
#endif

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

int
shutdown (int socket, int how)
{
  PTHREAD_UNSAFE

  if (__socket_valid (socket) == -1)
    return -1;

  return _shutdown ((int)__u->fd[socket].handle, how);
}
