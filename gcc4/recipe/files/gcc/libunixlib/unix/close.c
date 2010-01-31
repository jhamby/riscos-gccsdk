/* __close (), close ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#include <internal/unix.h>
#include <internal/os.h>
#include <internal/dev.h>
#include <internal/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

/* #define DEBUG */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

int
__close (struct __unixlib_fd *file_desc)
{
  if (__atomic_modify (&(file_desc->devicehandle->refcount), -1) == 0)
    {
      struct __sul_process *sulproc = __ul_global.sulproc;
      int x;

      /* We can close the file */
#ifdef DEBUG
      debug_printf (", closing\n");
#endif

      x = dev_funcall (file_desc->devicehandle->type, close, (file_desc));

      sulproc->sul_free (sulproc->pid, file_desc->devicehandle);

      /* Invalidate this file descriptor.  */
      file_desc->devicehandle = NULL;

      if (x == -1)
        {
#ifdef DEBUG
          debug_printf ("         Failed!: %s\n",
			_kernel_last_oserror()->errmess);
#endif
          return -1;
        }
    }
  else
    {
      /* Invalidate this file descriptor.  */
      file_desc->devicehandle = NULL;
#ifdef DEBUG
      debug_printf (", duplicate\n");
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
  debug_printf ("-- close(fd=%d): ", fd);
#endif

  if (BADF (fd))
    {
#ifdef DEBUG
      debug_printf ("bad file descriptor\n");
#endif
      return __set_errno (EBADF);
    }

  file_desc = getfd (fd);

  if (file_desc->devicehandle->type == DEV_SOCKET)
    FD_CLR (fd, &__socket_fd_set);

  return __close (file_desc);
}
