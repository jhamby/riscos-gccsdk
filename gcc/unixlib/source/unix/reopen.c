/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/reopen.c,v $
 * $Date: 2000/07/15 14:52:44 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: reopen.c,v 1.1.1.1 2000/07/15 14:52:44 nick Exp $";
#endif

#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/syslib.h>
#include <sys/unix.h>
#include <sys/dev.h>

#include <unixlib/fd.h>
#include <unixlib/local.h>

int
__reopen (int fd, const char *file, int oflag, ...)
{
  va_list ap;
  int mode;

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
  if (__close (fd) == -1)
    return -1;

  /* Now open the file.  */
  return __open (fd, file, oflag, mode);
}
