#include <stdio.h>
#include <errno.h>

FILE *fdopen (int fd, const char *mode)
{
  errno = ENOSYS;
  return NULL;
}
