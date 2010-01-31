/* gtty ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>
#include <sgtty.h>

/* Fill in *PARAMS with terminal parameters associated with FD.  */
int
gtty (int fd, struct sgttyb *params)
{
  return ioctl (fd, TIOCGETP, (void *) params);
}
