/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/socket.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.2 $
 * $State: Exp $
 * $Author: admin $
 *
 * (c) Copyright 1995 Sergio Monesi
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: socket.c,v 1.2.2.2 2001/09/04 16:32:04 admin Exp $";
#endif

#include <errno.h>

#include <unixlib/unix.h>
#include <sys/socket.h>
#include <unixlib/dev.h>
#include <fcntl.h>
#include <sys/netdb.h>

int
socket (int af, int type, int protocol)
{
  struct __unixlib_fd *file_desc;
  int fd, sd;

  if ((sd = _socket (af, type, protocol)) < 0)
    return -1;

  /* Set FIOSLEEPTW on sockets to allow multi-tasking when a blocking call
     is made in a task window.  This is potentially dangerous and considered by
     some (myself included [PJB]) to be out of place within this routine.  This
     code was originally part of the _socket.s file and I [PJB] have just moved
     it here for clarity.  To reduce the dangerous nature of this ioctl,
     we now only do it when we are executing within a task window.  */

  if (__taskwindow)
    {
      int arg = 1;
      int saved_errno = errno;

      if (_sioctl (sd, FIOSLEEPTW, &arg) < 0)
	{
	  if (errno != EOPNOTSUPP)
	    {
	      /* All going horribly wrong, so close socket (ignoring errors)
		 and return unexpected error from ioctl.  */
	      saved_errno = errno;
	      _sclose (sd);
	      return __set_errno (saved_errno);
	    }
	  else
	    /* FIOSLEEPTW not supported, so just carry on anyway.  */
	    (void) __set_errno (saved_errno);
	}
    }

  if ((fd = __alloc_file_descriptor ()) == -1)
    return -1;

  /* printf("U! socket: sd=%d fd=%d\n",sd,fd); */

  file_desc = &__u->fd[fd];
  file_desc->fflag = O_RDWR | O_BINARY;
  file_desc->dflag = 0;
  file_desc->handle = (void *) sd;
#if 0
  f->r[1] = af;
  f->r[2] = type;
  f->r[3] = protocol;
#endif

  file_desc->device = DEV_SOCKET;

  file_desc->__magic = _FDMAGIC;
  file_desc->pid = __u->pid;

  return fd;
}
