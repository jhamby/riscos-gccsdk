/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/getpeernam.c,v $
 * $Date: 2003/04/28 21:04:35 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: alex $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getpeernam.c,v 1.5 2003/04/28 21:04:35 alex Exp $";
#endif

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

int
getpeername (int s, __SOCKADDR_ARG name, socklen_t *__restrict namelen) __THROW
{
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

  return _getpeername ((int)__u->fd[s].handle, name, namelen);
}
