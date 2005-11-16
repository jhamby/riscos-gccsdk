/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/tcgetattr.c,v $
 * $Date: 2005/04/05 21:11:22 $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unixlib/unix.h>
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
