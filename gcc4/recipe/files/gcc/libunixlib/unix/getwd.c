/* Get the current working directory.
   Copyright (c) 2004-2011 UnixLib Developers.  */

#include <limits.h>
#include <unistd.h>

/* BSD version of getcwd.  */
char *
getwd (char *buffer)
{
  return getcwd (buffer, _POSIX_PATH_MAX);
}
