/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/accept.c,v $
 * $Date: 2004/04/21 19:43:20 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: accept.c,v 1.6 2004/04/21 19:43:20 alex Exp $";
#endif

#include <errno.h>

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/dev.h>
#include <fcntl.h>
#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>
#ifdef DEBUG
#include <stdio.h>
#endif

int
accept (int s, struct sockaddr *name, socklen_t *namelen)
{
  struct __unixlib_fd *file_desc;
  int nfd;
  int nsd;

  PTHREAD_UNSAFE_CANCELLATION

#ifdef DEBUG
  fprintf(stderr, "U! accept: fd=%d, realsocket=%d\n",s,(int)__u->fd[s].handle);
#endif
  if (__socket_valid (s) == -1)
    return -1;

  if ((nsd = _accept ((int)__u->fd[s].handle, name, namelen)) < 0)
    return (-1);

  if ((nfd = __alloc_file_descriptor ()) < 0)
    return (-1);

#ifdef DEBUG
  fprintf(stderr, "U! accept: nsd=%d nfd=%d\n",nsd,nfd);
#endif

  file_desc = &__u->fd[nfd];
  file_desc->fflag = O_RDWR;
  file_desc->dflag = 0;
  file_desc->handle = (void *)nsd;

  file_desc->device = DEV_SOCKET;

  file_desc->__magic = _FDMAGIC;
  file_desc->pid = __u->pid;

  FD_SET (nfd, &__socket_fd_set);

  return nfd;
}
