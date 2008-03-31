/* accept ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <internal/unix.h>
#include <internal/fd.h>
#include <internal/local.h>
#include <internal/dev.h>
#include <pthread.h>

int
accept (int s, struct sockaddr *name, socklen_t *namelen)
{
  struct __unixlib_fd *file_desc;
  const struct __sul_process *sulproc = __ul_global.sulproc;
  int nfd;
  int nsd;

  PTHREAD_UNSAFE_CANCELLATION

  if (__socket_valid (s) == -1)
    return -1;

  if ((nsd = _accept ((int)(getfd (s)->devicehandle->handle), name, namelen)) < 0)
    return -1;

  if ((nfd = __alloc_file_descriptor (0)) < 0)
    return -1;

  file_desc = getfd (nfd);
  file_desc->fflag = O_RDWR;
  file_desc->dflag = 0;

  file_desc->devicehandle = sulproc->sul_malloc (sulproc->pid, sulproc->fdhandlesize);
  if (file_desc->devicehandle == NULL)
    return __set_errno (ENOMEM);

  file_desc->devicehandle->handle = (void *)nsd;
  file_desc->devicehandle->type = DEV_SOCKET;
  file_desc->devicehandle->refcount = 1;

  FD_SET (nfd, &__socket_fd_set);

  return nfd;
}
