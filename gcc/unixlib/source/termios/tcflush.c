/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/termios/c/tcflush,v $
 * $Date: 1997/12/17 22:02:59 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: tcflush,v 1.1 1997/12/17 22:02:59 unixlib Exp $";
#endif

#include <errno.h>
#include <stddef.h>
#include <termios.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/ioctl.h>

/* Flush pending data on FD.  */
int
tcflush (int fd, int queue_selector)
{
  int arg;

  switch (queue_selector)
    {
    case TCIFLUSH:
      arg = FREAD;
      break;
    case TCOFLUSH:
      arg = FWRITE;
      break;
    case TCIOFLUSH:
      arg = FREAD | FWRITE;
      break;
    default:
      return __set_errno (EINVAL);
    }

  return ioctl (fd, TIOCFLUSH, (void *) &arg);
}
