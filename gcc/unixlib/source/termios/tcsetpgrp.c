/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/tcsetpgrp.c,v $
 * $Date: 2000/07/15 14:52:35 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: tcsetpgrp.c,v 1.1.1.1 2000/07/15 14:52:35 nick Exp $";
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
