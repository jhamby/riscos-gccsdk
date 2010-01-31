/* sigsuspend ()
 * Written by Nick Burrett, 26 Aug 1996.
 * Copyright (c) 1996-2008 UnixLib Developers
 */

#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <unistd.h>

#include <internal/unix.h>
#include <pthread.h>

/* #define DEBUG */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

/* Change the set of blocked signals to SET,
   wait until a signal arrives, and restore the set of blocked signals.  */
int
sigsuspend (const sigset_t * set)
{
  sigset_t oset;

  PTHREAD_SAFE_CANCELLATION

  if (set == NULL)
    return __set_errno (EINVAL);

  if (sigprocmask (SIG_SETMASK, set, &oset) < 0)
    return -1;

  /* Set the process sleeping.  Then wait until this is unset by something
     else.  A process calling sigsuspend will usually be woken up after
     delivery of a SIGSTOP signal or after any other signal and any pending
     ones have been delivered.  */
  __u->sleeping = 1;
  /* Bit of a silly busy wait this is. But unfortunately we aren't under a
     tasking system so it's not really possible to have a SWI Wimp_Poll
     call here.  */
#ifdef DEBUG
  debug_printf ("-- sigsuspend: Process suspended. Waiting for a signal.\r\n");
#endif
  while (__u->sleeping)
    pthread_yield ();
#ifdef DEBUG
  debug_printf ("-- sigsuspend: Signal received. Process continuing\r\n");
#endif

  if (sigprocmask (SIG_SETMASK, &oset, (sigset_t *) NULL) < 0)
    return -1;

  return __set_errno (EINTR);
}
