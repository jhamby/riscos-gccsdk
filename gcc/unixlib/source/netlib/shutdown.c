/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/shutdown.c,v $
 * $Date: 2003/12/29 19:02:39 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: peter $
 *
 * (c) Copyright 1997 Nicholas Clark
 ***************************************************************************/

#ifdef EMBED_RCSIS
static const char rcs_id[] = "$Id: shutdown.c,v 1.5 2003/12/29 19:02:39 peter Exp $";
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

  return _shutdown ((int)(getfd (ssocket)->devicehandle->handle), how);
}
