/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/stty.c,v $
 * $Date: 2000/07/15 14:52:35 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: stty.c,v 1.1.1.1 2000/07/15 14:52:35 nick Exp $";
#endif

#include <errno.h>
#include <sgtty.h>

/* Set the terminal parameters associated with FD to *PARAMS.  */
int
stty (int fd, const struct sgttyb *params)
{
  return ioctl (fd, TIOCSETP, (void *) params);
}
