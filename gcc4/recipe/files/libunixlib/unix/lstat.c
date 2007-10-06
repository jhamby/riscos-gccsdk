/* lstat (), lstat64 ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <sys/stat.h>

/* Difference between stat and lstat(64) is the support for symbolic
   links but seen that we don't have this on RISC OS, we can simply make
   lstat(64) call stat.  */
int
lstat (const char *filename, struct stat *buf)
{
  return stat (filename, buf);
}

int
lstat64 (const char *filename, struct stat64 *buf)
{
  /* struct stat64 is currently the same as struct stat.  */
  return stat (filename, (struct stat *)buf);
}
