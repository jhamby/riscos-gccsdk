/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/termios/c/tcgetpgrp,v $
 * $Date: 1997/12/17 22:02:59 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: tcgetpgrp,v 1.1 1997/12/17 22:02:59 unixlib Exp $";
#endif

#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

/* Return the foreground process group ID of FD.  */
pid_t
tcgetpgrp (int fd)
{
  int pgrp;

  if (ioctl (fd, TIOCGPGRP, &pgrp) < 0)
    return (pid_t) -1;

  return (pid_t) pgrp;
}
