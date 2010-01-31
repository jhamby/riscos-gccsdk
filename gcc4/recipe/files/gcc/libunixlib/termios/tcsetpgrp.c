/* tcsetpgrp ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

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
