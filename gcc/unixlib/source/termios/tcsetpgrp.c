/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/tcsetpgrp.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>

/* Set the foreground process group ID of FD set PGRP_ID.  */
int
tcsetpgrp (int fd, pid_t pgrp_id)
{
  return ioctl (fd, TIOCSPGRP, &pgrp_id);
}
