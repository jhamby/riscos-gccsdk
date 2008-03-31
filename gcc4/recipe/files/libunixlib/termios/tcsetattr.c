/* Set terminal attributes.
   Copyright (c) 2004, 2005, 2008 UnixLib Developers.  */

#include <errno.h>
#include <stddef.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include <internal/unix.h>

/* Set the state of FD to *TERMIOS_P.  */
int
tcsetattr (int fd, int optional_actions, const struct termios *termios_p)
{
  if (termios_p == NULL)
    return __set_errno (EINVAL);

  switch (optional_actions & ~TCSASOFT)
    {
    case TCSANOW:
      break;
    case TCSADRAIN:
      if (tcdrain (fd) < 0)
	return -1;
      break;
    case TCSAFLUSH:
      if (tcflush (fd, TCIFLUSH) < 0)
	return -1;
      break;
    default:
      return __set_errno (EINVAL);
    }

  if (termios_p->c_ispeed > __MAX_BAUD || termios_p->c_ospeed > __MAX_BAUD)
    return __set_errno (EINVAL);

  if (ioctl (fd, TIOCSETA, (struct termios *) termios_p) < 0)
    return -1;

  return 0;
}
