/* __unixlib_signal_initialise ()
 * Initialise the UnixLib stack handlers.
 * Written by Nick Burrett, 26 August 1996.
 * Copyright (c) 1996-2008 UnixLib Developers
 */

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include <internal/sigstate.h>
#include <internal/unix.h>
#include <internal/os.h>

void
__unixlib_signal_initialise (struct proc *p)
{
  int signo;
  struct unixlib_sigstate *ss = &p->sigstate;

#ifdef DEBUG
  __os_print ("__unixlib_signal_initialise\r\n");
#endif

  /* Initialise signal handlers.  */
  sigemptyset (&ss->actions[0].sa_mask);
  ss->actions[0].sa_flags = SA_RESTART;
  ss->actions[0].sa_handler = SIG_DFL;

  /* Set all signals to their defaults.  */
  for (signo = 1; signo < NSIG; ++signo)
    ss->actions[signo] = ss->actions[0];

  ss->currently_handling = 0;
  ss->signalstack.ss_sp = 0;
  ss->signalstack.ss_size = 0;
  ss->signalstack.ss_flags = SA_DISABLE;
}
