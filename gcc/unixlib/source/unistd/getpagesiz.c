/* Get memory page size.
   Copyright (c) 2005 UnixLib Developers.  */

#include <unistd.h>
#include <unixlib/unix.h>

/* The system page size is held as a cached value, generated during
   UnixLib initialisation.  */
int getpagesize (void)
{
  return __ul_pagesize;
}
