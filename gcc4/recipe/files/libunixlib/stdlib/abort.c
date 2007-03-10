/* abort ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void
abort (void)
{
  sigset_t sigs;

  PTHREAD_UNSAFE

  sigs = sigmask (SIGABRT);
  sigprocmask (SIG_UNBLOCK, &sigs, NULL);
  raise (SIGABRT);
  /* If the signal fails, exit.  */
  _exit (127);
}
