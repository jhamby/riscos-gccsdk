/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/send.c,v $
 * $Date: 2003/04/28 21:04:36 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: alex $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: send.c,v 1.5 2003/04/28 21:04:36 alex Exp $";
#endif

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

int
send (int s, const void *msg, size_t len, int flags)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (__socket_valid (s) == -1)
    return -1;

/*  printf("U! send: fd=%d, realsocket=%d\n",s,(__u->file+s)->r[0]); */
  return _send ((int)(getfd (s)->devicehandle->handle), msg, len, flags);
}
