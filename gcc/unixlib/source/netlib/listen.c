/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/listen.c,v $
 * $Date: 2002/02/14 15:56:36 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: listen.c,v 1.3 2002/02/14 15:56:36 admin Exp $";
#endif

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

int
listen (int s, int backlog)
{
  PTHREAD_UNSAFE

  if (__socket_valid (s) == -1)
    return -1;

/*  printf("U! listen: fd=%d, realsocket=%d\n",s,(__u->file+s)->r[0]); */
  return _listen ((int)__u->fd[s].handle, backlog);
}
