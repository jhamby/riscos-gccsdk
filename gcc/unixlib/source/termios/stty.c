/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/termios/c/stty,v $
 * $Date: 1997/12/17 22:02:58 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: stty,v 1.1 1997/12/17 22:02:58 unixlib Exp $";
#endif

#include <errno.h>
#include <sgtty.h>

/* Set the terminal parameters associated with FD to *PARAMS.  */
int
stty (int fd, const struct sgttyb *params)
{
  return ioctl (fd, TIOCSETP, (void *) params);
}
