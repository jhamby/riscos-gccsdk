/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/tcflush.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

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
