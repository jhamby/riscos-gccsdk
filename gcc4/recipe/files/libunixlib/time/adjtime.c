/* adjtime ()
 * Copyright (c) 2000-2007 UnixLib Developers
 */

#include <errno.h>
#include <sys/time.h>

/* This function speeds up or slows down the system clock in order to
   make gradual adjustments in the current time.  This ensures that
   the time reported by the system clock is always monotonically
   increasing, which might not happen if you simply set the current
   time.  Irrelevant on RISC OS.  */

int
adjtime (const struct timeval *delta, struct timeval *olddelta)
{
  return __set_errno (ENOSYS);
}
