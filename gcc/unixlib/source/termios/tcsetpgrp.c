/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/termios/c/tcsetpgrp,v $
 * $Date: 1997/12/17 22:02:59 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: tcsetpgrp,v 1.1 1997/12/17 22:02:59 unixlib Exp $";
#endif

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
