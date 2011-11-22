/* creat (), creat64 ()
 * Copyright (c) 2000-2011 UnixLib Developers
 */

#include <fcntl.h>
#include <unixlib/types.h>

int
creat (const char *file, __mode_t mode)
{
  return open (file, O_WRONLY | O_CREAT | O_TRUNC, mode);
}

int
creat64 (const char *file, __mode_t mode)
{
  return open64 (file, O_WRONLY | O_CREAT | O_TRUNC, mode);
}

