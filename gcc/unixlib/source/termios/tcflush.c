/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/tcflush.c,v $
 * $Date: 2000/07/15 14:52:35 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: tcflush.c,v 1.1.1.1 2000/07/15 14:52:35 nick Exp $";
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
