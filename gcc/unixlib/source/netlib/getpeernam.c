/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/getpeernam.c,v $
 * $Date: 2005/03/04 20:59:05 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: alex $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getpeernam.c,v 1.7 2005/03/04 20:59:05 alex Exp $";
#endif

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

int
getpeername (int s, __SOCKADDR_ARG name, socklen_t *__restrict namelen)
{
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

  return _getpeername ((int)(getfd (s)->devicehandle->handle), name, namelen);
}
