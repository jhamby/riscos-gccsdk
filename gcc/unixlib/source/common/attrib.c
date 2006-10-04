/* __set_protection (), __get_protection ()
 * Written by Nick Burrett, 3 May 1997.
 * Copyright (c) 1997-2006 UnixLib Developers
 */

#include <sys/stat.h>
#include <unixlib/types.h>
#include <unixlib/local.h>

/* Convert a file mode into a attribute mask suitable for
   passing to RISC OS.  */
int __set_protection (__mode_t mode)
{
  int attribute;

  attribute = (mode & S_IREAD) ? (1 << 0) : 0; /* Owner read */
  attribute |= (mode & S_IWRITE) ? (1 << 1) : 0; /* Owner write */
  attribute |= (mode & S_IEXEC) ? (1 << 2) : 0; /* Owner execute */
  attribute |= ((mode & S_IROTH) || (mode & S_IRGRP)) ? (1 << 4) : 0; /* Public read */
  attribute |= ((mode & S_IWOTH) || (mode & S_IWGRP)) ? (1 << 5) : 0; /* Public write */
  return attribute;
}

/* Convert a RISC OS attribute mask into a 'stat' compatible
   file access mode.  */
__mode_t __get_protection (int attribute)
{
  __mode_t mode;

  mode = (attribute & (1 << 0)) ? S_IREAD : 0; /* Owner read */
  mode |= (attribute & (1 << 1)) ? S_IWRITE : 0; /* Owner write */
  mode |= (attribute & (1 << 2)) ? S_IEXEC : 0; /* Owner execute */
  mode |= (attribute & (1 << 4)) ? (S_IROTH | S_IRGRP) : 0; /* Public read */
  mode |= (attribute & (1 << 5)) ? (S_IWOTH | S_IWGRP) : 0; /* Public write */
  return mode;
}
