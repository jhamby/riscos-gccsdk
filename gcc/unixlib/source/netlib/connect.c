/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/connect.c,v $
 * $Date: 2003/04/28 21:04:35 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: alex $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: connect.c,v 1.7 2003/04/28 21:04:35 alex Exp $";
#endif

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

int
connect (int s, const struct sockaddr *name, socklen_t namelen)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (__socket_valid (s) == -1)
    return -1;

  return _connect ((int)(getfd (s)->devicehandle->handle), name, namelen);
}
