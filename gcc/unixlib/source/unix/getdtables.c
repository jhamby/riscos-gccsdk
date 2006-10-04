/* getdtablesize ()
 * Written by Nick Burrett, 13 October 1996.
 * Copyright (c) 1996-2006 UnixLib Developers
 */

#include <limits.h>
#include <unistd.h>

/* Return the maximum number of file descriptors
   the current process could possibly have.  */

/* A stub function that really should take into account the
   actual number of files open on RISC OS and determine whether
   that OPEN_MAX files can still be opened. Also should take
   into account getrusage ().  */

int
getdtablesize (void)
{
  return OPEN_MAX;
}
