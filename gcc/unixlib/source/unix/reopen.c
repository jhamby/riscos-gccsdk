/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/reopen,v $
 * $Date: 1997/10/10 19:38:58 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: reopen,v 1.4 1997/10/10 19:38:58 unixlib Exp $";
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
