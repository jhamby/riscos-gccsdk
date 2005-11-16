/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/init.c,v $
 * $Date: 2002/02/14 15:56:36 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* Initialise the UnixLib stack handlers.
   Written by Nick Burrett, 26 August 1996.  */

#include <stdlib.h>
#include <signal.h>
#include <unixlib/sigstate.h>
#include <unixlib/unix.h>
#include <unistd.h>
#include <unixlib/os.h>

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
