/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/accept.c,v $
 * $Date: 2005/11/12 17:45:17 $
 * $Revision: 1.9 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

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
  fprintf(stderr, "U! accept: fd=%d, realsocket=%d\n",s,(int)(getfd (s)->devicehandle->handle));
#endif
  if (__socket_valid (s) == -1)
    return -1;

  if ((nsd = _accept ((int)(getfd (s)->devicehandle->handle), name, namelen)) < 0)
    return (-1);

  if ((nfd = __alloc_file_descriptor (0)) < 0)
    return (-1);

#ifdef DEBUG
  fprintf(stderr, "U! accept: nsd=%d nfd=%d\n",nsd,nfd);
#endif

  file_desc = getfd (nfd);
  file_desc->fflag = O_RDWR;
  file_desc->dflag = 0;

  file_desc->devicehandle = __proc->sul_malloc (__proc->pid, __proc->fdhandlesize);
  if (file_desc->devicehandle == NULL)
    return -1;

  file_desc->devicehandle->handle = (void *)nsd;
  file_desc->devicehandle->type = DEV_SOCKET;
  file_desc->devicehandle->refcount = 1;

  FD_SET (nfd, &__socket_fd_set);

  return nfd;
}
