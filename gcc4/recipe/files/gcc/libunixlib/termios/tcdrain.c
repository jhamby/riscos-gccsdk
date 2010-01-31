/* tcdrain ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>
#include <stddef.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>


/* Wait for pending output to be written on FD.  */
int
tcdrain (int fd)
{
  return 0;
}
