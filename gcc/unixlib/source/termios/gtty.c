/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/gtty.c,v $
 * $Date: 2000/07/15 14:52:35 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: gtty.c,v 1.1.1.1 2000/07/15 14:52:35 nick Exp $";
#endif

#include <errno.h>
#include <sgtty.h>

/* Fill in *PARAMS with terminal parameters associated with FD.  */
int
gtty (int fd, struct sgttyb *params)
{
  return ioctl (fd, TIOCGETP, (void *) params);
}
