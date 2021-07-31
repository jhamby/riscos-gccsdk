/* tcgetpgrp ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

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
