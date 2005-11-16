/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/isatty.c,v $
 * $Date: 2005/03/04 20:59:06 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <errno.h>
#include <unistd.h>
#include <unixlib/dev.h>
#include <unixlib/unix.h>
#include <unixlib/fd.h>
#include <pthread.h>

/* Return 1 if fd is a tty.  */
int
isatty (int fd)
{
  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  return getfd (fd)->devicehandle->type == DEV_TTY;
}
