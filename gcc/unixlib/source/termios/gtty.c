/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/gtty.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <errno.h>
#include <sgtty.h>

/* Fill in *PARAMS with terminal parameters associated with FD.  */
int
gtty (int fd, struct sgttyb *params)
{
  return ioctl (fd, TIOCGETP, (void *) params);
}
