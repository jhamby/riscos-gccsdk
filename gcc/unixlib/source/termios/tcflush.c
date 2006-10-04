/* tcflush ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

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
