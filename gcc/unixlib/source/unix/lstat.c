/* lstat (), lstat64 ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <sys/stat.h>

int
lstat (const char *fname, struct stat *buf)
{
  return stat (fname, buf);
}

int
lstat64 (const char *fname, struct stat *buf)
{
  return stat (fname, buf);
}
