/* socket ()
 * Copyright (c) 1995 Sergio Monesi
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <errno.h>
#include <fcntl.h>
#ifndef __TARGET_SCL__
#  include <pthread.h>
#endif
#include <netdb.h>
#include <sys/socket.h>

#ifndef __TARGET_SCL__
#  include <internal/dev.h>
#  include <internal/unix.h>
#endif

int
socket (int af, int type, int protocol)
{
#ifdef __TARGET_SCL__
  return _socket (af, type, protocol);
#else
  PTHREAD_UNSAFE

  int sd;
  if ((sd = _socket (af, type, protocol)) < 0)
    return -1;

  /* Set FIOSLEEPTW on sockets to allow multi-tasking when a blocking call
     is made in a task window.  This is potentially dangerous and considered by
     some (myself included [PJB]) to be out of place within this routine.  This
     code was originally part of the _socket.s file and I [PJB] have just moved
     it here for clarity.  To reduce the dangerous nature of this ioctl,
     we now only do it when we are executing within a task window.  */

  const struct ul_global *gbl = &__ul_global;
  if (gbl->taskwindow)
    {
      int arg = 1;
      int saved_errno = errno;

      if (_sioctl (sd, FIOSLEEPTW, &arg) < 0)
	{
	  if (errno != EOPNOTSUPP)
	    {
	      /* All going horribly wrong, so close socket (ignoring errors)
		 and return unexpected error from ioctl.  */
	      _sclose_no_error (sd);
	      return -1;
	    }
	  else
	    /* FIOSLEEPTW not supported, so just carry on anyway.  */
	    (void) __set_errno (saved_errno);
	}
    }

  int fd;
  if ((fd = __alloc_file_descriptor (0)) == -1)
    {
      _sclose_no_error (sd);
      return -1;
    }

  struct __unixlib_fd *file_desc = getfd (fd);
  file_desc->fflag = O_RDWR;
  file_desc->dflag = 0;

  const struct __sul_process *sulproc = gbl->sulproc;
  file_desc->devicehandle = sulproc->sul_malloc (sulproc->pid,
						 sulproc->fdhandlesize);
  if (file_desc->devicehandle == NULL)
    {
      /* Because file_desc->devicehandle remains NULL, file_desc is still
         marked as free and should not be freed explicitely.  */
      _sclose_no_error (sd);
      return __set_errno (ENOMEM);
    }

  file_desc->devicehandle->handle = (void *) sd;

  file_desc->devicehandle->type = DEV_SOCKET;
  file_desc->devicehandle->refcount = 1;

  FD_SET (fd, &__socket_fd_set);

  return fd;
#endif
}
