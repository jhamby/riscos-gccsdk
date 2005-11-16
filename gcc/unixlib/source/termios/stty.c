/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/stty.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <errno.h>
#include <sgtty.h>

/* Set the terminal parameters associated with FD to *PARAMS.  */
int
stty (int fd, const struct sgttyb *params)
{
  return ioctl (fd, TIOCSETP, (void *) params);
}
