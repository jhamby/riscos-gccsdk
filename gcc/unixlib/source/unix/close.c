/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/close,v $
 * $Date: 1997/10/10 19:38:57 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: close,v 1.7 1997/10/10 19:38:57 unixlib Exp $";
#endif

#include <errno.h>
#include <fcntl.h>

#include <sys/syslib.h>
#include <sys/types.h>
#include <sys/unix.h>
#include <sys/dev.h>
#include <sys/os.h>
#include <unistd.h>

#include <unixlib/fd.h>
#include <unixlib/local.h>

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
	  os_print (", closing\r\n");
#endif
	  x = __funcall ((*(__dev[file_desc->device].close)), (file_desc));
	  if (x == -1)
	    {
#ifdef DEBUG
	      os_print ("         Failed!: ");
	      os_print (_kernel_last_oserror()->errmess);
	      os_nl();
#endif
	      return -1;
	    }
	}
    }
#ifdef DEBUG
    else
      {
	os_print (", duplicate\r\n");
      }
#endif
  /* Invalidate this file descriptor.  */
  file_desc->__magic = 0;

  return 0;
}

int
close (int fd)
{
#ifdef DEBUG
  os_print ("close(): fd = ");
  os_prhex (fd);
#endif

  if (BADF (fd))
    {
#ifdef DEBUG
      os_print (" - bad file descriptor\r\n");
      __write_corefile(1);
#endif
      return __set_errno (EBADF);
    }

  return __close (fd);
}
