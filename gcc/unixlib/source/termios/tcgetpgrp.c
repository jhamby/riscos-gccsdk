/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/tcgetpgrp.c,v $
 * $Date: 2000/07/15 14:52:35 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: tcgetpgrp.c,v 1.1.1.1 2000/07/15 14:52:35 nick Exp $";
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
