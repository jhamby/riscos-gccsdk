/* sigblock ()
 * BSD signal functions.
 * Written by Nick Burrett, 5 October 1996.
 * Copyright (c) 1996-2006 UnixLib Developers
 */

#include <signal.h>

/* Block signals in MASK, returning the old mask.  */
int
sigblock (int mask)
{
  sigset_t set, oset;

  set = (sigset_t) mask;

  if (sigprocmask (SIG_BLOCK, &set, &oset) < 0)
    return -1;

  return (int) oset;
}
