/* UnixLib ttyname_r() implementation.
   Copyright (c) 2010 UnixLib Developers.  */

#include <errno.h>
#include <unistd.h>

#include <internal/dev.h>
#include <internal/fd.h>
#include <internal/unix.h>

int
ttyname_r (int fd, char *buf, size_t buflen)
{
  if (buf == NULL)
    return __set_errno (EINVAL);

  if (buflen < sizeof ("/dev/tty"))
    return __set_errno (ERANGE);

  if (BADF (fd))
    return __set_errno (EBADF);

  if (getfd (fd)->devicehandle->type != DEV_TTY)
    return __set_errno (ENOTTY);

  strcpy (buf, "/dev/tty");
  return 0;
}

