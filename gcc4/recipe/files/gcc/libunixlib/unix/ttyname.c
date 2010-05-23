/* UnixLib ttyname() implementation.
   Copyright (c) 2010 UnixLib Developers.  */

#include <pthread.h>
#include <unistd.h>

#include <internal/unix.h>

char *
ttyname (int fd)
{
  static char cttyname[sizeof ("/dev/tty")];

  PTHREAD_UNSAFE

  return ttyname_r (fd, cttyname, sizeof (cttyname)) < 0 ? cttyname : NULL;
}
