/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/termios/c/tcdrain,v $
 * $Date: 1997/12/17 22:02:59 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: tcdrain,v 1.1 1997/12/17 22:02:59 unixlib Exp $";
#endif

#include <errno.h>
#include <stddef.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>


/* Wait for pending output to be written on FD.  */
int
tcdrain (int fd)
{
  /* The TIOCSETP control waits for pending output to be written before
     affecting its changes, so we use that without changing anything.  */
  struct sgttyb b;
  if (ioctl (fd, TIOCGETP, (void *) &b) < 0 ||
      ioctl (fd, TIOCSETP, (void *) &b) < 0)
    return -1;
  return 0;
}
