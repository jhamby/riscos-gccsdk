/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/tcsendbrk.c,v $
 * $Date: 2000/07/15 14:52:35 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: tcsendbrk.c,v 1.1.1.1 2000/07/15 14:52:35 nick Exp $";
#endif

#include <errno.h>
#include <stddef.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/types.h>

/* Send zero bits on FD.  */
int
tcsendbreak (int fd, int duration)
{
  struct timeval delay;

  /* The break lasts 0.25 to 0.5 seconds if DURATION is zero,
     and an implementation-defined period if DURATION is nonzero.
     We define a positive DURATION to be number of microseconds to break.  */
  if (duration <= 0)
    duration = 400000;

  delay.tv_sec = 0;
  delay.tv_usec = duration;

  /* Starting sending break.  */
  if (ioctl (fd, TIOCSBRK, NULL) < 0)
    return -1;

  /* Wait DURATION microseconds.  */
  select (0, NULL, NULL, NULL, &delay);

  /* Turn off the break.  */
  return ioctl (fd, TIOCCBRK, NULL);
}
