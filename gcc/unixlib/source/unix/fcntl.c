/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/fcntl.c,v $
 * $Date: 2003/06/23 09:30:11 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fcntl.c,v 1.7 2003/06/23 09:30:11 peter Exp $";
#endif

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <netdb.h>
#include <unixlib/unix.h>
#include <unixlib/fd.h>
#include <unixlib/dev.h>
#include <pthread.h>

int
fcntl (int fd, int cmd, ...)
{
  va_list ap;
  struct __unixlib_fd *file_desc;

  PTHREAD_UNSAFE_CANCELLATION

  if (BADF (fd))
    return __set_errno (EBADF);

  file_desc = &__u->fd[fd];

  switch (cmd)
    {
    case F_DUPFD:
      {
	/* Duplicate the file descriptor.  */
	int duplicate_fd;

	va_start (ap, cmd);
	duplicate_fd = va_arg (ap, int);
	va_end (ap);

	/* Check the new file descriptor for validity.  */
	if ((unsigned int) duplicate_fd < MAXFD)
	  {
	    if (duplicate_fd == 0)
	      /* Allocate a new file descriptor.  */
	      duplicate_fd = __alloc_file_descriptor ();

	    if (duplicate_fd != -1)
	      {
		__u->fd[duplicate_fd] = __u->fd[fd];
		/* File descriptor flags aren't duplicated.  */
		__u->fd[duplicate_fd].dflag = 0;
	      }
	    return duplicate_fd;
	  }
	else
	  return __set_errno (EINVAL);
      }

    case F_GETFD:
      return (file_desc->fflag & O_EXECCL) ? 1 : 0;

    case F_SETFD:
      {
      /* Set close-on-exec flag */
            va_start(ap, cmd);

	if (va_arg (ap, int))
	  file_desc->fflag |= O_EXECCL;
	else
	  file_desc->fflag &= ~O_EXECCL;
		va_end(ap);
	return 0;
	}

    case F_GETFL:
      return file_desc->fflag;

    case F_SETFL:
      {
	int modify = O_APPEND | O_NONBLOCK | O_ASYNC;
	int newfflag;

	va_start (ap, cmd);
        newfflag = va_arg (ap, int);
        va_end (ap);

        if (file_desc->device == DEV_SOCKET)
          {
             if ((file_desc->fflag ^ newfflag) & O_NONBLOCK)
	       {
		 int arg = (newfflag & O_NONBLOCK) ? 1 : 0;
		 _sioctl((int)file_desc->handle, FIONBIO, &arg);
	       }

             if ((file_desc->fflag ^ newfflag) & O_ASYNC)
	       {
		 int arg = (newfflag & O_ASYNC) ? 1 : 0;
		 _sioctl((int)file_desc->handle, FIOASYNC, &arg);
	       }
          }

        file_desc->fflag = (file_desc->fflag & ~modify) | (newfflag & modify);
      }
      return 0;

    case F_GETUNL:
      return (file_desc->fflag & O_UNLINKED) ? 1 : 0;

    case F_SETUNL:
      {
	int arg;

	va_start (ap, cmd);
	arg = va_arg (ap, int);
	va_end (ap);

	if (arg)
	  file_desc->fflag |= O_UNLINKED;
	else
	  file_desc->fflag &= ~O_UNLINKED;
	return 0;
      }

    case F_SETOWN:
      {
	return 0;
      }
    }

  return __set_errno (EINVAL);
}
