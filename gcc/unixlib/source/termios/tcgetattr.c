/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/tcgetattr.c,v $
 * $Date: 2005/03/04 20:59:06 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: tcgetattr.c,v 1.7 2005/03/04 20:59:06 alex Exp $";
#endif

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
