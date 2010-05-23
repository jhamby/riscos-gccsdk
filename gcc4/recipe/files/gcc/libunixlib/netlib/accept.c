/* accept ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <errno.h>
#include <fcntl.h>
#ifndef __TARGET_SCL__
#  include <pthread.h>
#endif
#include <sys/socket.h>

#ifndef __TARGET_SCL__
#  include <internal/dev.h>
#  include <internal/unix.h>
#  include <internal/local.h>
#endif

int
accept (int s, struct sockaddr *name, socklen_t *namelen)
{
#ifdef __TARGET_SCL__
  return _accept (s, name, namelen);
#else
  PTHREAD_UNSAFE_CANCELLATION

  if (__socket_valid (s) == -1)
    return -1;

  int nsd;
  if ((nsd = _accept ((int)getfd (s)->devicehandle->handle, name, namelen)) < 0)
    return -1;

  int nfd;
  if ((nfd = __alloc_file_descriptor (0)) < 0)
    {
      _sclose_no_error (nsd);
      return -1;
    }

  struct __unixlib_fd *file_desc = getfd (nfd);
  file_desc->fflag = O_RDWR;
  file_desc->dflag = 0;

  const struct __sul_process *sulproc = __ul_global.sulproc;
  file_desc->devicehandle = sulproc->sul_malloc (sulproc->pid, sulproc->fdhandlesize);
  if (file_desc->devicehandle == NULL)
    {
      /* Because file_desc->devicehandle remains NULL, file_desc is still
         marked as free and should not be freed explicitely.  */
      _sclose_no_error (nsd);
      return __set_errno (ENOMEM);
    }

  file_desc->devicehandle->handle = (void *)nsd;
  file_desc->devicehandle->type = DEV_SOCKET;
  file_desc->devicehandle->refcount = 1;

  FD_SET (nfd, &__socket_fd_set);

  return nfd;
#endif
}
