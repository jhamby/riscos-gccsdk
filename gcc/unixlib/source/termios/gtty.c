/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/termios/c/gtty,v $
 * $Date: 1997/12/17 22:02:58 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: gtty,v 1.1 1997/12/17 22:02:58 unixlib Exp $";
#endif

#include <errno.h>
#include <sgtty.h>

/* Fill in *PARAMS with terminal parameters associated with FD.  */
int
gtty (int fd, struct sgttyb *params)
{
  return ioctl (fd, TIOCGETP, (void *) params);
}
