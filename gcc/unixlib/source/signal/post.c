/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/signal/c/post,v $
 * $Date: 2000/06/03 14:46:10 $
 * $Revision: 1.12 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: post,v 1.12 2000/06/03 14:46:10 admin Exp $";
#endif

/* signal.c.post: Written by Nick Burrett, 27 August 1996.  */

#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#include <sys/os.h>
#include <sys/unix.h>
#include <unixlib/sigstate.h>

/* #define DEBUG */

void
__unixlib_default_sigaction (struct unixlib_sigstate *ss)
{
  int signo;

  sigemptyset (&ss->actions[0].sa_mask);
  ss->actions[0].sa_flags = SA_RESTART;
  ss->actions[0].sa_handler = SIG_DFL;

  /* Set all signals to their defaults.  */
  for (signo = 1; signo < NSIG; ++signo)
    ss->actions[signo] = ss->actions[0];
}


void
__unixlib_internal_post_signal (struct unixlib_sigstate *ss, int signo)
{
  enum
  {
    stop, ignore, core, term, handle
  }
  act;
  __sighandler_t handler;
  sigset_t pending, signal_mask;
  int ss_suspended;

  /* 0 is the special signo used for posting any pending signals.  */
  if (signo == 0)
    goto post_pending;

post_signal:
#ifdef DEBUG
  os_print ("__unixlib_internal_post_signal: sigstate = ");
  os_prhex ((int) ss); os_print (", signo = ");
  os_prdec (signo); os_print ("\r\n");
#endif

  signal_mask = sigmask (signo);

  /* Increment the number of signals this process has received.  */
  __u->usage.ru_nsignals++;

  handler = SIG_DFL;
  ss_suspended = 0;

  if (handler != SIG_DFL)
    /* Run the preemption-provided handler.  */
    act = handle;
  else
    {
      /* Do normal handling.  */

      handler = ss->actions[signo].sa_handler;
#ifdef DEBUG
      os_print ("__unixlib_internal_post_signal: handler = ");
      os_prhex ((int) handler); os_print ("\r\n");
#endif

      if (handler == SIG_DFL)
        {
	  /* Figure out the default action for this signal.  */
	  __sigset_t mask;

	  /* Unrecognised signals will cause process termination.  */
	  act = term;

	  /* Check for signals that will stop a process.  */
	  mask = sigmask (SIGTTIN) | sigmask (SIGTTOU)
	        | sigmask (SIGSTOP) | sigmask (SIGTSTP);
	  if (signal_mask & mask)
	    act = stop;
	  else
	    {
	      /* Check signals that can be otherwise ignored.  */
	      mask = sigmask (SIGCONT) | sigmask (SIGIO) | sigmask (SIGURG)
		    | sigmask (SIGCHLD) | sigmask (SIGWINCH);
	      if (signal_mask & mask)
	        act = ignore;
	      else
		{
		  /* Fatal signals that will cause a core dump.  */
		  mask = sigmask (SIGQUIT) | sigmask (SIGILL)
			| sigmask (SIGTRAP) | sigmask (SIGIOT)
			| sigmask (SIGEMT) | sigmask (SIGFPE)
			| sigmask (SIGBUS) | sigmask (SIGSEGV)
			| sigmask (SIGSYS);
		  if (signal_mask & mask)
		    act = core;
		  else if (signo == SIGINFO)
		    {
		      if (__u->pgrp == __u->pid)
			{
			  /* We provide a default handler for SIGINFO since
			     there is no user-specified handler.  */
			  act = handle;
			  handler = __unixlib_siginfo_handler;
			}
		      else
			act = ignore;
		    }
		}
	    }
	}
      else if (handler == SIG_IGN)
	act = ignore;
      else
	act = handle;

      if (signal_mask & (sigmask (SIGTTIN) | sigmask (SIGTTOU)
			 | sigmask (SIGSTOP) | sigmask (SIGTSTP)))
	/* Stop signals clear a pending SIGCONT even if they
	   are handled or ignored (but not if preempted).  */
	sigdelset (&ss->pending, SIGCONT);
      else if (signo == SIGCONT)
	{
	  /* Even if handled or ignored (but not preempted),
	     SIGCONT clears stop signals and resumes the process.  */
	  sigdelset (&ss->pending, SIGTTIN);
	  sigdelset (&ss->pending, SIGTTOU);
	  sigdelset (&ss->pending, SIGSTOP);
	  sigdelset (&ss->pending, SIGTSTP);
	  /* Resume all our children.  */
	  ss_suspended = 1;
	  __u->status.stopped = 0;
	}
    }

#ifdef DEBUG
  os_print ("__unixlib_internal_post_signal: act = ");
  os_prdec (act); os_print ("\r\n");
#endif

  if (__u->orphaned && act == stop &&
      (signal_mask & (sigmask (SIGTTIN) | sigmask (SIGTTOU) | sigmask (SIGTSTP))))
    {
      /* If we would ordinarily stop for a job control signal, but we are
         orphaned so noone would ever notice and continue us again, we just
         quietly die, alone and in the dark.  */
      signo = SIGKILL;
      act = term;
    }

  /* Handle a blocked signal.  */
  if ((sigismember (&ss->blocked, signo) && act != ignore)
      || (signo != SIGKILL && __u->stopped))
    {
      sigaddset (&ss->pending, signo);
      act = ignore;
    }

  /* Perform the chosen action for the signal.  */
  switch (act)
    {
    case stop:
#ifdef DEBUG
      os_print ("__unixlib_internal_post_signal: stop\r\n");
#endif
      if (!__u->status.stopped)
	{
	  /* Stop all our threads, and mark ourselves stopped.  */
	  __u->status.stopped = 1;
	}
      /* Wake up sigsuspend. */
      __u->sleeping = 0; /* inline version of sigwakeup() */
      break;

    case ignore:
#ifdef DEBUG
      os_print ("__unixlib_internal_post_signal: ignore\r\n");
#endif
      /* Nobody cares about this signal.  */
      break;

    case term:			/* Time to die.  */
    case core:			/* and leave a rotting corpse.  */
    death:
      /* Stop all other threads in our task.  */
      /* No more user instructions wil be executed. */
      {
	int status = W_EXITCODE (0, signo);
	/* Do a core dump if desired. Only set the wait status bit saying
	   we in fact dumped core if the operation was actually successful.  */
#ifdef DEBUG
	os_print ("__unixlib_internal_post_signal: term/core\r\n");
#endif
	if (act == term)
	  __write_termination (signo);
	else if (act == core && __write_corefile (signo))
	  status |= WCOREFLAG;

	/* Die, returning information about how we died.  */
	_exit (status);
	/* Never reached.  */
      }
      break;

    case handle:
      /* Call a handler for this signal.  */
      {
	sigset_t blocked;
	int flags;
#ifdef DEBUG
	os_print ("__unixlib_internal_post_signal: handle\r\n");
#endif
	/* We're going to handle this signal now, so remove it from
	   the pending list.  */
	sigdelset (&ss->pending, signo);

	/* Block SIGNO and requested signals while running the handler.  */
	blocked = ss->blocked;
	ss->blocked |= sigmask (signo) | ss->actions[signo].sa_mask;
	flags = ss->actions[signo].sa_flags;
	/* Re-instate the default signal handler.  We do this before executing
	   the signal handler because a new handler might be setup whilst
	   executing the signal handler.  */
	ss->actions[signo].sa_handler = SIG_DFL;
	ss->actions[signo].sa_flags = SA_RESTART;
	sigemptyset (&ss->actions[signo].sa_mask);

	/* Call the function to set the thread up to run the signal
	   handler, and preserve its old context.  */
	if (__unixlib_setup_sighandler (ss, handler, signo, flags))
	  {
	    /* We got a fault setting up the stack frame for the handler.
	       Nothing to do but die.  */
	    signo = SIGQUIT;
	    act = core;
	    goto death;
	  }
	/* If we reach here, we have successfully executed the signal handler.
	   All that is left is to restore the defaults.  */

	/* Re-instate the original sigset.  */
	ss->blocked = blocked;
	break;
      }
    }

  /* Deliver pending signals.  */

post_pending:
#ifdef DEBUG
  os_print ("__unixlib_internal_post_signal: Deliver pending signals\r\n");
#endif
  if (!__u->stopped && (pending = ss->pending & ~ss->blocked))
    {
      for (signo = 1; signo < NSIG; ++signo)
	if (sigismember (&pending, signo))
	  {
	    sigdelset (&ss->pending, signo);
	    goto post_signal;
	  }
    }

  ss->currently_handling = 0;
#ifdef DEBUG
  os_print ("__unixlib_internal_post_signal: pending signals delivered\r\n");
#endif
  /* No more signals pending.  */
  __u->sleeping = 0; /* inline version of sigwakeup (); */
}

void
__unixlib_raise_signal (struct unixlib_sigstate *ss, int signo)
{
  if (ss == NULL)
    ss = &__u->sigstate;

  /* Mark signo as pending to be delivered.  */
  if (signo != 0)
    {
      sigaddset (&ss->pending, signo);
    }

  if (!ss->currently_handling)
    {
      /* Post the signal if we are not already handling a signal.  */
      ss->currently_handling = 1;
      __unixlib_internal_post_signal (ss, signo);
    }
#ifdef DEBUG
  else
    {
      os_print ("\n\r__unixlib_raise_signal: signal ");
      os_print (sys_siglist[signo]);
      os_print (" is pending for delivery\r\n");
    }
#endif
}
