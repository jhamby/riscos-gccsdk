/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/reopen.c,v $
 * $Date: 2003/04/05 09:33:57 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: reopen.c,v 1.4 2003/04/05 09:33:57 alex Exp $";
#endif

#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <unixlib/unix.h>
#include <unixlib/dev.h>

#include <unixlib/fd.h>
#include <unixlib/local.h>
#include <pthread.h>

int
__reopen (int fd, const char *file, int oflag, ...)
{
  va_list ap;
  int mode;

  PTHREAD_UNSAFE

  if (BADF (fd))
    return __set_errno (EBADF);

  if (oflag & O_CREAT)
    {
      va_start (ap, oflag);
      mode = va_arg (ap, int);
      va_end (ap);
    }
  else
    mode = 0777;

  /* First close the file.  */
  if (__close (getfd (fd)) == -1)
    return -1;

  /* Now open the file.  */
  return __open (fd, file, oflag, mode);
}
