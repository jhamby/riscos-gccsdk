/* mkstemp ()
 * Copyright (c) 2000-2012 UnixLib Developers
 */

#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int
mkstemp (char *file_template)
{
  return open (mktemp (file_template), O_RDWR | O_CREAT | O_EXCL, 0600);
}
#if __UNIXLIB_LFS64_SUPPORT
#  error "64-bit LFS support missing."
#else
/* FIXME: indicate 4 GByte-1 limit.  */
strong_alias (mkstemp, mkstemp64)
#endif

