/* __close (), close ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

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

int
__close (struct __unixlib_fd *file_desc)
{
  if (__atomic_modify (&(file_desc->devicehandle->refcount), -1) == 0)
    {
      struct __sul_process *sulproc = __ul_global.sulproc;
      int x;
      /* We can close the file */
#ifdef DEBUG
      __os_print (", closing\r\n");
#endif

      x = dev_funcall (file_desc->devicehandle->type, close, (file_desc));

      sulproc->sul_free (sulproc->pid, file_desc->devicehandle);

      /* Invalidate this file descriptor.  */
      file_desc->devicehandle = NULL;

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
  else
    {
      /* Invalidate this file descriptor.  */
      file_desc->devicehandle = NULL;
#ifdef DEBUG
      __os_print (", duplicate\r\n");
#endif
    }

  return 0;
}

int
close (int fd)
{
  struct __unixlib_fd *file_desc;

  PTHREAD_UNSAFE_CANCELLATION

#ifdef DEBUG
  __os_print ("close(): fd = ");
  __os_prhex (fd);
#endif

  if (BADF (fd))
    {
#ifdef DEBUG
      __os_print (" - bad file descriptor\r\n");
      __unixlib_raise_signal(NULL, SIGILL);
#endif
      return __set_errno (EBADF);
    }

  file_desc = getfd (fd);

  if (file_desc->devicehandle->type == DEV_SOCKET)
    FD_CLR (fd, &__socket_fd_set);

  return __close (file_desc);
}
