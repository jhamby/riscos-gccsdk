/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/close.c,v $
 * $Date: 2002/02/14 15:56:38 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: close.c,v 1.3 2002/02/14 15:56:38 admin Exp $";
#endif

#include <errno.h>
#include <fcntl.h>

#include <sys/types.h>
#include <unixlib/unix.h>
#include <unixlib/dev.h>
#include <unixlib/os.h>
#include <unistd.h>

#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

/* #define DEBUG */

int __close (int fd)
{
  struct __unixlib_fd *f, *file_desc;
  int x, duplicate;

  f = &__u->fd[0];
  file_desc = &__u->fd[fd];

  /* Check for any duplicated file descriptors, recognisable by
     an equivalent 'handle' field.  */
  duplicate = 0;
  for (x = 0; x < MAXFD; x++)
    {
      if (x != fd && f[x].__magic == _FDMAGIC
	  && f[x].handle == file_desc->handle)
	{
	  duplicate = 1;
	  break;
	}
    }

  if (!duplicate)
    {
      /* We can close the file, providing we are the process that
	 opened it.  */
      if (file_desc->pid == __u->pid)
	{
#ifdef DEBUG
	  __os_print (", closing\r\n");
#endif
	  x = __funcall ((*(__dev[file_desc->device].close)), (file_desc));
	  if (x == -1)
	    {
#ifdef DEBUG
	      __os_print ("         Failed!: ");
	      __os_print (_kernel_last_oserror()->errmess);
	      __os_nl();
#endif
	      return -1;
	    }
	}
    }
#ifdef DEBUG
    else
      {
	__os_print (", duplicate\r\n");
      }
#endif
  /* Invalidate this file descriptor.  */
  file_desc->__magic = 0;

  return 0;
}

int
close (int fd)
{
  PTHREAD_UNSAFE_CANCELLATION

#ifdef DEBUG
  __os_print ("close(): fd = ");
  __os_prhex (fd);
#endif

  if (BADF (fd))
    {
#ifdef DEBUG
      __os_print (" - bad file descriptor\r\n");
      __write_corefile(1);
#endif
      return __set_errno (EBADF);
    }

  return __close (fd);
}
