/* sigsetmask ()
 * BSD signal blocking function.
 * Written by Nick Burrett, 5 October 1996.
 * Copyright (c) 1996-2006 UnixLib Developers
 */

#include <signal.h>

/* Set the mask of blocked signals to MASK, returning the old mask.  */
int
sigsetmask (int mask)
{
  sigset_t set, oset;

  set = (sigset_t) mask;

  if (sigprocmask (SIG_SETMASK, &set, &oset) < 0)
    return -1;

  return (int) oset;
}
