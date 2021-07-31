/* stty ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>
#include <sgtty.h>

/* Set the terminal parameters associated with FD to *PARAMS.  */
int
stty (int fd, const struct sgttyb *params)
{
  return ioctl (fd, TIOCSETP, (void *) params);
}
