/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/open.c,v $
 * $Date: 2002/01/31 14:32:04 $
 * $Revision: 1.2.2.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: open.c,v 1.2.2.3 2002/01/31 14:32:04 admin Exp $";
#endif

#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <unixlib/unix.h>
#include <unixlib/dev.h>

#include <unixlib/fd.h>
#include <unixlib/local.h>

/* Map of special device names to device types.  */
struct sfile
{
  const char *name;
  const dev_t dev;
};

static const struct sfile __sfile[] =
{
  {"tty", DEV_TTY},
  {"console", DEV_TTY},
  {"rs423", DEV_TTY},
  {"null", DEV_NULL},
/*   {"pipe", DEV_PIPE}, does open on /dev/pipe make sense ?  */
/*   {"socket", DEV_SOCKET}, does open on /dev/socket make sense ?  */
  {NULL, DEV_RISCOS} /* table terminator */
};

char *
ttyname (int fd)
{
  static char name[10];

  if (BADF (fd))
    {
      (void) __set_errno (EBADF);
      return NULL;
    }

  if (__u->fd[fd].device == DEV_TTY)
    return strcpy (name, "/dev/tty");

  return NULL;
}

/* This function will open a file on the specified file descriptor.
   There is no error checking.  */
int
__open (int fd, const char *file, int oflag, int mode)
{
  struct __unixlib_fd *file_desc;

  file_desc = &__u->fd[fd];
  file_desc->fflag = oflag;
  file_desc->dflag = 0;

  /* Perform a special check for devices.  */
  if (file[0] == '/' && file[1] == 'd' && file[2] == 'e' &&
      file[3] == 'v' && file[4] == '/')
    {
      const struct sfile *special = __sfile;

      while (special->name != NULL)
	{
	  if (strcmp (special->name, file + sizeof ("/dev/") - 1) == 0)
	    break;
	  special++;
	}
      file_desc->device = special->dev;
    }
  else
    file_desc->device = DEV_RISCOS;

  /* Perform the device specific open operation.  */
  file_desc->handle = __funcall ((*(__dev[file_desc->device].open)),
				 (file_desc, file, mode));
  if (file_desc->handle == (void *) -1)
    return -1;

  file_desc->pid = __u->pid;
  /* Set the magic number right at the end. This saves us invalidating
     it during the previous error checking.  */
  file_desc->__magic = _FDMAGIC;

  return fd;
}

int
open (const char *file, int oflag, ...)
{
  va_list ap;
  int mode, fd;

  if (file == NULL)
    return __set_errno (EINVAL);

/* __riscosify() is called by __fsopen() */

#ifdef DEBUG
  __os_print ("open(): file = ");
  __os_print (file);
  __os_print ("\r\n");
#endif

  if (oflag & O_CREAT)
    {
      va_start (ap, oflag);
      mode = va_arg (ap, int);
      va_end (ap);
    }
  else
    mode = 0777;

  /* Allocate a new file descriptor. This function automatically
     sets errno if there are none available.  */
  fd = __alloc_file_descriptor ();
  if (fd == -1)
    return -1;

  return __open (fd, file, oflag, mode);
}
