/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/post.c,v $
 * $Date: 2001/09/06 14:52:00 $
 * $Revision: 1.2.2.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: post.c,v 1.2.2.3 2001/09/06 14:52:00 admin Exp $";
#endif

/* signal.c.post: Written by Nick Burrett, 27 August 1996.  */

#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#include <unixlib/os.h>
#include <unixlib/unix.h>
#include <unixlib/sigstate.h>

/* #define DEBUG 1 */

/* This function chooses a suitable execution environment
   for the signal handler and calls the appropriate function
   to perform the job.

   Return 0 on sucess, 1 on fail.  */
static int
sigsetup (struct unixlib_sigstate *ss, sighandler_t handler,
	  int signo, int flags)
{
  int system_stack;

#ifdef DEBUG
  __os_print ("sigsetup: signo = ");
  __os_prdec (signo); __os_print (", handler = "); __os_prhex ((int) handler);
  __os_print ("\r\n");
#endif
  system_stack = ((ss->signalstack.ss_flags & SA_DISABLE)
		  || ss->signalstack.ss_size == 0
		  || ss->signalstack.ss_sp == 0) ? 1 : 0;

  /* The user has requested to execute the signal on a sigstack
     but it can only be executed on the system stack. Return failure.  */
  if ((flags & SA_ONSTACK) && system_stack)
    {
#ifdef DEBUG
      __os_print ("sigsetup: bad system stack\r\n");
      __os_print ("   ss_size = "); __os_prdec ((int) ss->signalstack.ss_size);
      __os_print (", ss_sp = "); __os_prdec ((int) ss->signalstack.ss_sp);
      __os_print (", ss_flags = "); __os_prdec ((int) ss->signalstack.ss_flags);
      __os_print ("\r\n");
#endif
      return 1;
    }

  /* If the SA_DISABLE flag is set, we will not execute
     this signal on a signal stack.

     Look out for problems where the stack pointer is zero or the
     stack size is zero.  */
  if (system_stack)
    {
#ifdef DEBUG
      __os_print (" unixlib_setup_sighandler: executing off normal stack\r\n");
#endif
      __unixlib_exec_sig (handler, signo);
      return 0;
    }

  /* Specify that the signal stack is currently in use.  */
  ss->signalstack.ss_flags |= SA_ONSTACK;

  /* If ss_size == -1, then this sigaltstack was the result of
     a BSD sigstack conversion. Run the BSD alternative signal
     executor.  */
  if (ss->signalstack.ss_size == -1)
    {
#ifdef DEBUG
      __os_print (" unixlib_setup_sighandler: executing off BSD stack\r\n");
#endif
      __unixlib_exec_sigstack_bsd (ss->signalstack.ss_sp, handler, signo);
    }
  else
    {
#ifdef DEBUG
      __os_print (" unixlib_setup_sighandler: executing off POSIX stack\r\n");
#endif
      __unixlib_exec_sigstack (ss->signalstack.ss_sp, ss->signalstack.ss_size,
			       handler, signo);
    }
  /* Say that the signal stack is no longer in use.  */
  ss->signalstack.ss_flags &= ~SA_ONSTACK;

  return 0;
}


static void
post_signal (struct unixlib_sigstate *ss, int signo)
{
  static const unsigned int stop_signals = sigmask (SIGTTIN)
    | sigmask (SIGTTOU) | sigmask (SIGSTOP) | sigmask (SIGTSTP);

  static const unsigned int ignore_signals = sigmask (SIGCONT)
    | sigmask (SIGIO) | sigmask (SIGURG) | sigmask (SIGCHLD)
    | sigmask (SIGWINCH);
  
  static const unsigned int core_signals = sigmask (SIGQUIT) | sigmask (SIGILL)
    | sigmask (SIGTRAP) | sigmask (SIGIOT) | sigmask (SIGEMT)
    | sigmask (SIGFPE)	| sigmask (SIGBUS) | sigmask (SIGSEGV)
    | sigmask (SIGSYS);
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
  __os_print ("post_signal: sigstate = ");
  __os_prhex ((int) ss); __os_print (", signo = ");
  __os_prdec (signo); __os_print ("\r\n");
#endif

  /* Increment the number of signals this process has received.  */
  __u->usage.ru_nsignals++;

  handler = SIG_DFL;
  ss_suspended = 0;
  signal_mask = sigmask (signo);

  if (handler != SIG_DFL)
    /* Run the preemption-provided handler.  */
    act = handle;
  else
    {
      /* Do normal handling.  */

      handler = ss->actions[signo].sa_handler;
#ifdef DEBUG
      __os_print ("post_signal: handler = ");
      __os_prhex ((int) handler); __os_print ("\r\n");
#endif

      if (handler == SIG_DFL)
        {
	  /* Unrecognised signals will cause process termination.  */
	  act = term;

	  /* Check for signals that will stop a process.  */
	  if (signal_mask & stop_signals)
	    act = stop;
	  /* Check signals that can be otherwise ignored.  */
	  else if (signal_mask & ignore_signals)
	    act = ignore;
	  /* Fatal signals that will cause a core dump.  */
	  else if (signal_mask & core_signals)
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
      else if (handler == SIG_IGN)
	act = ignore;
      else
	act = handle;

      if (signal_mask & stop_signals)
	/* Stop signals clear a pending SIGCONT even if they
	   are handled or ignored (but not if preempted).  */
	sigdelset (&ss->pending, SIGCONT);
      else if (signo == SIGCONT)
	{
	  /* Even if handled or ignored (but not preempted),
	     SIGCONT clears stop signals and resumes the process.  */
	  ss->pending &= ~stop_signals;
	  /* Resume all our children.  */
	  ss_suspended = 1;
	  __u->status.stopped = 0;
	}
    }

#ifdef DEBUG
  __os_print ("post_signal: act = ");
  __os_prdec (act); __os_print ("\r\n");
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
      __os_print ("post_signal: stop\r\n");
#endif
      /* Stop all our threads, and mark ourselves stopped.  */
      __u->status.stopped = 1;
      /* Wake up sigsuspend. */
      __u->sleeping = 0; /* inline version of sigwakeup() */
      break;

    case ignore:
#ifdef DEBUG
      __os_print ("post_signal: ignore\r\n");
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
	__os_print ("post_signal: term/core\r\n");
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
	__os_print ("post_signal: handle\r\n");
#endif
	/* We're going to handle this signal now, so remove it from
	   the pending list.  */
	sigdelset (&ss->pending, signo);

	/* Block SIGNO and requested signals while running the handler.  */
	blocked = ss->blocked;
	ss->blocked |= signal_mask | ss->actions[signo].sa_mask;
	flags = ss->actions[signo].sa_flags;
	/* Re-instate the default signal handler.  We do this before executing
	   the signal handler because a new handler might be setup whilst
	   executing the signal handler.  */
	ss->actions[signo].sa_handler = SIG_DFL;
	ss->actions[signo].sa_flags = SA_RESTART;
	sigemptyset (&ss->actions[signo].sa_mask);

	/* Call the function to set the thread up to run the signal
	   handler, and preserve its old context.  */
	if (sigsetup (ss, handler, signo, flags))
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
  __os_print ("post_signal: Deliver pending signals\r\n");
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
  __os_print ("post_signal: pending signals delivered\r\n");
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
      post_signal (ss, signo);
    }
#ifdef DEBUG
  else
    {
      __os_print ("\n\r__unixlib_raise_signal: signal ");
      __os_print (sys_siglist[signo]);
      __os_print (" is pending for delivery\r\n");
    }
#endif
}
