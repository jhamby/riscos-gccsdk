/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/tcgetpgrp.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

/* Return the foreground process group ID of FD.  */
pid_t
tcgetpgrp (int fd)
{
  int pgrp;

  if (ioctl (fd, TIOCGPGRP, &pgrp) < 0)
    return (pid_t) -1;

  return (pid_t) pgrp;
}
