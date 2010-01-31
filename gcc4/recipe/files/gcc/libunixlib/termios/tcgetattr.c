/* tcgetattr ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include <internal/unix.h>
#include <pthread.h>

/* Put the state of FD into *TERMIOS_P.  */
int
tcgetattr (int fd, struct termios *termios_p)
{
  PTHREAD_UNSAFE

  if (termios_p == NULL)
    return __set_errno (EINVAL);

  if (ioctl(fd, TIOCGETA, termios_p) < 0)
    return -1;

  return 0;
}
