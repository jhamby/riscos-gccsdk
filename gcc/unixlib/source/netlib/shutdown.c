/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/shutdown.c,v $
 * $Date: 2003/04/28 21:04:36 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: alex $
 *
 * (c) Copyright 1997 Nicholas Clark
 ***************************************************************************/

#ifdef EMBED_RCSIS
static const char rcs_id[] = "$Id: shutdown.c,v 1.4 2003/04/28 21:04:36 alex Exp $";
#endif

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

int
shutdown (int ssocket, int how)
{
  PTHREAD_UNSAFE

  if (__socket_valid (ssocket) == -1)
    return -1;

  return _shutdown ((int)__u->fd[ssocket].handle, how);
}
