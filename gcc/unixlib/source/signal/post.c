/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/post.c,v $
 * $Date: 2005/04/04 12:01:15 $
 * $Revision: 1.23 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: post.c,v 1.23 2005/04/04 12:01:15 peter Exp $";
#endif

/* signal.c.post: Written by Nick Burrett, 27 August 1996.  */

#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <swis.h>
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
  __os_nl ();
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
      __os_nl ();
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
  if ((int)ss->signalstack.ss_size == -1)
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
      __unixlib_exec_sigstack (ss->signalstack.ss_sp,
			       ss->signalstack.ss_size, handler, signo);
    }
  /* Say that the signal stack is no longer in use.  */
  ss->signalstack.ss_flags &= ~SA_ONSTACK;

  return 0;
}


/* Returns non-zero value when address range <lower> - <upper> (excl) is
   a valid address range.  */
int
__valid_address (const void *lower, const void *upper)
{
  int flags;

  return !(_swix(OS_ValidateAddress,
	 _INR(0,1) | _OUT(_FLAGS), lower, upper, &flags) || (flags & _C) != 0);
}


static void
write_termination (int signo)
{
  fprintf(stderr, "\nTermination signal received: %s\n", sys_siglist[signo]);
}


void
__write_backtrace (int signo)
{
  int features;
  int *fp = __backtrace_getfp(), *oldfp = NULL;

  if (_swix(OS_PlatformFeatures, _IN(0) | _OUT(0), 0, &features))
    features = 0;

  /* The ASM version did originally disable environment handlers
     but this seems to cause problems.  */

  fprintf(stderr, "\nFatal signal received: %s\n\nStack backtrace:\n\n",
	  strsignal(signo));

  /* fp[0]  => pc
     fp[-1] => lr
     fp[-2] => sp
     fp[-3] => previous fp  */
  while (fp != NULL)
    {
      unsigned int *pc, *lr;

      /* Check that FP is different.  */
      if (fp == oldfp)
	{
	  fprintf(stderr, "fp unchanged at %x\n", (int)fp);
	  break;
	}

      /* Validate FP address.  */
      if (!__valid_address(fp - 3, fp))
	{
	  fprintf(stderr, "Stack frame has gone out of bounds "
			  "with address %x\n", (int)fp - 12);
	  break;
	}

      /* Retrieve PC counter.  */
      if (__32bit)
	pc = (unsigned int *)(fp[0] & 0xfffffffc);
      else
	pc = (unsigned int *)(fp[0] & 0x03fffffc);
      if (!(features & 0x8))
	pc += 1;

      if (!__valid_address(pc, pc))
	{
	  fprintf(stderr, "Invalid pc address %x\n", (int)pc);
	  break;
	}

      /* Retrieve lr.  */
      if (__32bit)
	lr = (unsigned int *)(fp[-1] & 0xfffffffc);
      else
	lr = (unsigned int *)(fp[-1] & 0x03fffffc);

      fprintf(stderr, "  (%8x) pc: %8x lr: %8x sp: %8x ",
	      (int)fp, (int)pc, (int)lr, fp[-2]);

      /* Retrieve function name.  */
      if (!__valid_address(pc - 7, pc))
	{
	  fputs("[invalid address]\n", stderr);
	}
      else
	{
	  int address;
	  const char *name = NULL;

	  for (address = -3; address > -8; address--)
	    {
	      if ((pc[address] & 0xffffff00) == 0xff000000)
		{
		  name = (const char *)(pc + address) - (pc[address] & 0xff);
		  break;
		}
	    }

	  /* Function name sanity check.  */
	  if (name != NULL
	      && (!__valid_address(name, (name + 256))
		  || strnlen(name, 256) == 256))
	    name = NULL;

	  if (!name)
	    fputs(" ?()\n", stderr);
	  else
	    fprintf(stderr, " %s()\n", name);
	}

      oldfp = fp;
      fp = (int *)fp[-3];

      if (fp == __ul_callbackfp && fp != NULL)
	{
	  int reg;
	  const char * const rname[16] =
	    { "a1", "a2", "a3", "a4", "v1", "v2", "v3", "v4",
	      "v5", "v6", "sl", "fp", "ip", "sp", "lr", "pc" };

	  /* At &oldfp[1] = cpsr, a1-a4, v1-v6, sl, fp, ip, sp, lr, pc */
	  fprintf(stderr, "\n  Register dump at %08x:\n", &oldfp[1]);

	  if (__valid_address(oldfp, oldfp + 17))
	    {
	    for (reg = 0; reg < 16; reg++)
	      {
		if ((reg & 0x3) == 0)
		  fputs("\n   ", stderr);

		fprintf(stderr, " %s: %8x", rname[reg], oldfp[reg + 2]);
	      }

	    if (__32bit)
	      fprintf(stderr, "\n  cpsr: %8x\n", oldfp[1]);
	    else
	      fprintf(stderr, "\n");
	    }
	  else
	    fputs("    [bad register dump address]\n", stderr);

          pc = (unsigned int *)oldfp[17];

          /* Try LR if PC invalid */
          if (pc < (unsigned int *)0x8000  || !__valid_address(pc - 5, pc + 3))
            pc = (unsigned int *)oldfp[16];

          if (pc > (unsigned int *)0x8000 && __valid_address(pc - 5, pc + 3))
            {
              unsigned int *diss;

              for (diss = pc - 5; diss <= pc + 3; diss++)
                {
                  const char *ins;
                  int length;

                  _swix(Debugger_Disassemble, _INR(0,1) | _OUTR(1,2), *diss, diss, &ins, &length);

                   fprintf(stderr, "\n    %08x    ", (unsigned int) diss);
                   fwrite(ins, length, 1, stderr);
                }
            }
          else
           {
             fprintf(stderr, "[Disassembly not available]"); 
           }

         fprintf(stderr, "\n\n");
       }
    }

  fputs("\n", stderr);
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
    | sigmask (SIGFPE) | sigmask (SIGBUS) | sigmask (SIGSEGV)
    | sigmask (SIGSYS);
  enum
  {
    stop, ignore, core, term, handle
  }
  act;
  __sighandler_t handler;
  sigset_t pending, signal_mask;
  int ss_suspended;
  int errbuf_valid;

  /* The error buffer is only valid for this signal and not the next */
  errbuf_valid = __ul_errbuf.valid;
  __ul_errbuf.valid = 0;

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
      __os_prhex ((int) handler); __os_nl ();
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
	      if (__proc->pgrp == __proc->pid)
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
	  __proc->status.stopped = 0;
	  __proc->status.reported = 0;
	}
    }

#ifdef DEBUG
  __os_print ("post_signal: act = ");
  __os_prdec (act); __os_print ("\r\n");
#endif

  if (__proc->ppid == 1 && act == stop &&
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
      __proc->status.stopped = 1;
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
      /* No more user instructions will be executed. */
      {
	int regs[10];
	int status = W_EXITCODE (0, signo);
	/* Do a core dump if desired. Only set the wait status bit saying
	   we in fact dumped core if the operation was actually successful.  */
#ifdef DEBUG
	__os_print ("post_signal: term/core\r\n");
#endif

	/* Update __taskhandle (The task could have called Wimp_Initialise
	   since we last checked at program startup */
	regs[0] = 3;
	if (__os_swi (Wimp_ReadSysInfo, regs))
	  regs[0] = 0;
	if (regs[0])
	  {
	    regs[0] = 5;
	    if (__os_swi (Wimp_ReadSysInfo, regs))
	      regs[0] = 0;
	  }
	__taskhandle = regs[0];

	if (__taskhandle && !__taskwindow && isatty (fileno (stderr)))
	  {
	    regs[0] = (int)__u->argv[0];
	    __os_swi (Wimp_CommandWindow, regs);
	  }

	if (errbuf_valid)
	  {
	    fprintf(stderr, "\nError 0x%x: %s\n  pc: %08x\n",
			    __ul_errbuf.errnum, __ul_errbuf.errmess,
			    (int *)__ul_errbuf.pc - 1);
	  }
	if (signo == SIGFPE)
	  {
	    fprintf(stderr, "  f0: %f  f1: %f  f2: %f  f3: %f\n"
			    "  f4: %f  f5: %f  f6: %f  f7: %f\n  fpsr: %x\n",
			    __ul_fp_registers.f[0], __ul_fp_registers.f[1],
			    __ul_fp_registers.f[2], __ul_fp_registers.f[3],
			    __ul_fp_registers.f[4], __ul_fp_registers.f[5],
			    __ul_fp_registers.f[5], __ul_fp_registers.f[7],
			    __ul_fp_registers.fpsr);
	  }

	if (act == term)
	  {
	    write_termination (signo);
	  }
	else if (act == core)
	  {
	    __write_backtrace (signo);
	    status |= WCOREFLAG;
	  }

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
  __os_print ("  pending 0x"); __os_prhex((int)ss->pending);
  __os_print (", blocked 0x"); __os_prhex((int)ss->blocked);
  __os_nl ();
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
  const char *err;

  PTHREAD_UNSAFE

#ifdef DEBUG
  __os_print("\r\n__unixlib_raise_signal: state is 0x");
  __os_prhex ((int)ss); __os_print(", signo is ");
  __os_prdec (signo); __os_nl ();
#endif

  /* When signals are raised, we could be in any state imaginable.  Be
     extra careful and verify the basic UnixLib ingredients before
     delivery.

     It is unlikely that we can recover from errors here, so print an
     error message and die.  */
  if (__u == NULL)
    {
      err = "UnixLib process structure is non-existant.";
      goto error;
    }

  if (__u != NULL && ! __valid_address (&__u[0], &__u[1]))
    {
      err = "UnixLib process structure is corrupt.";
      goto error;
    }

  if (ss == NULL)
    ss = &__u->sigstate;

  if (ss == NULL)
    {
      err = "UnixLib process structure is corrupt.";
      goto error;
    }

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
  else
    {
      /* We could be looping on a segfault.  Something bad may have
	 happened.  So we better report an error and exit fast.  */
      if (signo == SIGSEGV)
	{
	  err = "UnixLib detected recursion of signal SIGSEGV.";
	  goto error;
	}

#ifdef DEBUG
      __os_print ("\r\n__unixlib_raise_signal: signal ");
      __os_print (sys_siglist[signo]);
      __os_print (" is pending for delivery\r\n");
#endif
    }

  return;
 error:
  __os_nl ();
  __os_print (err);
  __os_print ("  Exiting.");
  __os_nl ();
  _exit (W_EXITCODE (0, signo));
}
