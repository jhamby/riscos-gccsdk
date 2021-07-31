/* tcflow ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>
#include <stddef.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>


/* Suspend or restart transmission on FD.  */
int
tcflow (int fd, int action)
{
  switch (action)
    {
    case TCOOFF:
      return ioctl (fd, TIOCSTOP, (void *) NULL);
    case TCOON:
      return ioctl (fd, TIOCSTART, (void *) NULL);

    case TCIOFF:
    case TCION:
      {
	/* This just writes the START or STOP character with
	   `write'.  Is there another way to do this?  */
	struct termios attr;
	unsigned char c;
	if (tcgetattr (fd, &attr) < 0)
	  return -1;
	c = attr.c_cc[action == TCIOFF ? VSTOP : VSTART];
	if (c != _POSIX_VDISABLE && write (fd, &c, 1) < 1)
	  return -1;
	return 0;
      }
    }
  return __set_errno (EINVAL);
}
