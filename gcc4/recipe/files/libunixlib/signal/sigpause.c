/* sigpause ()
 * BSD signal function.
 * Written by Nick Burrett, 5 October 1996.
 * Copyright (c) 1996-2006 UnixLib Developers
 */

#include <signal.h>
#include <pthread.h>

/* Set the mask of blocked signals to MASK,
   wait for a signal to arrive, and then restore the mask.  */
int
sigpause (int mask)
{
  sigset_t set;

  PTHREAD_SAFE_CANCELLATION

  set = (sigset_t) mask;

  return sigsuspend (&set);
}
