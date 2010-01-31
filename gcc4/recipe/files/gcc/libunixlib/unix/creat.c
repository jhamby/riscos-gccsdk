/* creat ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <fcntl.h>
#include <unixlib/types.h>

int
creat (const char *file, __mode_t mode)
{
  return open (file, O_WRONLY | O_CREAT | O_TRUNC, mode);
}
