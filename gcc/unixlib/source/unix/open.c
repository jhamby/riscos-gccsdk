/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/open.c,v $
 * $Date: 2004/12/11 14:18:57 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: open.c,v 1.7 2004/12/11 14:18:57 joty Exp $";
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
#include <pthread.h>

/* #define DEBUG */

char *
ttyname (int fd)
{
  static char name[10];

  PTHREAD_UNSAFE

  if (BADF (fd))
    {
      (void) __set_errno (EBADF);
      return NULL;
    }

  if (getfd (fd)->devicehandle->type == DEV_TTY)
    return strcpy (name, "/dev/tty");

  return NULL;
}

/* This function will open a file on the specified file descriptor.
   There is no error checking.  */
int
__open (int fd, const char *file, int oflag, int mode)
{
  struct __unixlib_fd *file_desc;

  PTHREAD_UNSAFE

  file_desc = getfd (fd);
  file_desc->fflag = oflag;
  file_desc->dflag = 0;

  file_desc->devicehandle = __proc->sul_malloc (__proc->pid, __proc->fdhandlesize);
  if (file_desc->devicehandle == NULL)
    return -1;

  /* Perform a special check for devices.  */
  file_desc->devicehandle->type = __getdevtype (file);
  file_desc->devicehandle->refcount = 1;

  /* Perform the device specific open operation.  */
  file_desc->devicehandle->handle = dev_funcall (file_desc->devicehandle->type,
				 open, (file_desc, file, mode));
  if (file_desc->devicehandle->handle == (void *) -1)
    {
      __proc->sul_free (__proc->pid, file_desc->devicehandle);
      file_desc->devicehandle = NULL;
      return -1;
    }

  return fd;
}

int
open (const char *file, int oflag, ...)
{
  va_list ap;
  int mode, fd;

  PTHREAD_UNSAFE_CANCELLATION

  if (file == NULL)
    return __set_errno (EINVAL);

/* __riscosify() is called by __fsopen() */

#ifdef DEBUG
  __os_print ("open(): file = ");
  __os_print (file);
  __os_nl ();
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
