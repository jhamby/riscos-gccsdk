/* Perform delivery of a signal to a process.
   Written by Nick Burrett.
   Copyright (c) 1996-2015 UnixLib Developers.  */

#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <swis.h>
#include <inttypes.h>

#include <unixlib/local.h>
#include <internal/os.h>
#include <internal/unix.h>
#include <internal/sigstate.h>
#include <internal/swiparams.h>

#include <ucontext.h>

/* #define DEBUG 1*/
#ifdef DEBUG
#  include <sys/debug.h>
#endif

#ifdef __ARM_EABI__
typedef enum abort_error
{
  abort_ERROR_NONE,		/* No error, last abort was handled correctly.  */
  abort_ERROR_ABORT_HANDLER,	/* There was an abort in the abort handler.  */
  abort_ERROR_STACK_OVERFLOW,	/* The abort was because of stack overflow.  */
  abort_ERROR_STACK_INTERNAL,	/* A SWI gave an error in the stack abort handler.  */
  abort_ERROR_MMAP_READ_ONLY,	/* Abort in mmap area with valid page (probably attempted write to read only page).  */
  abort_ERROR_MMAP_INTERNAL,	/* A SWI gave an error in mmap abort handler.  */
  abort_ERROR_MAX
} abort_error;

static const char *abort_msg[] =
{
  "Unknown error code (%d) from abort handler in ARMEABISupport\n\n",
  "Unixlib detected recursion in the abort handler of ARMEABISupport\n\n",
  "Unixlib detected stack overflow\n\n",
  "Unixlib detected that a SWI returned an error in the stack abort handler\n\n",
  "Unixlib detected an abort in an mmap area\n\n",
  "Unixlib detected that a SWI returned an error in the mmap abort handler\n\n"
};
#endif

static inline unsigned int
__signal_have_saved_regs (int signo)
{
  return signo == SIGSEGV || signo == SIGBUS || signo == SIGILL;
}

/* Given an address to the approximate start of a function, try
   to obtain an embedded function name.  */
static const char *
extract_name (const unsigned int *pc,
	      int *cplusplus_name)
{
  const char *name = NULL;

  if (cplusplus_name)
    *cplusplus_name = 0;

  if (__valid_address (pc - 7, pc + 1))
    {
      for (int address = 0; address > -8; address--)
	{
	  if ((pc[address] & 0xffffff00) == 0xff000000)
	    {
	      name = (const char *)(pc + address) - (pc[address] & 0xff);

	      /* Function name sanity check.  */
	      if (__valid_address (name, name + 256) && strnlen (name, 256) < 256)
		{
		  static char *demangled;
		  static size_t size;
		  int status;

		  const char *tdname = name[0] == '^' || name[0] == '#' ? name + 1 : name;
		  demangled = __unixlib_cxa_demangle (tdname, demangled,
						      &size, &status);
		  if (demangled != NULL && status == 0)
		    {
		      name = demangled;
		      if (cplusplus_name)
			*cplusplus_name = 1;
		    }
		}
	      else
		name = NULL;
	      break;
	    }
	}
    }

  if (name == NULL)
    name = "?";

  return name;
}

/* This function chooses a suitable execution environment
   for the signal handler and calls the appropriate function
   to perform the job.

   Return 0 on sucess, 1 on fail.  */
static int
sigsetup (struct unixlib_sigstate *ss, sighandler_t handler,
	  int signo, int flags)
{
  struct ul_memory *mem = &__ul_memory;

  /* Cast the function pointer to a standard void pointer here to reduce
     compiler warnings and number of places we have to apply this cast.  */
  unsigned int handler_addr = (unsigned int) handler;

  /* Perform an address validation on the user supplied handler address.
     We select a reasonable range of 16 instructions.  */

#ifdef PIC
  /* In the shared library, the application's address range can't be used
     to validate the handler code location. The handler may well be in a
     shared library as is the case with the UnixLib handlers. All we
     can do is ask the OS to validate.  */
  if (! __valid_address ((void *) handler_addr,
			 (void *) (handler_addr + 16 * 4)))
#else
  /* In the static library, the handler must also reside within the address
     range of read only code of the application.  */
  if (handler_addr < mem->robase
      || handler_addr > mem->rwbase
      || ! __valid_address ((void *) handler_addr,
			    (void *) (handler_addr + 16 * 4)))
#endif
    {
#ifdef DEBUG
      debug_printf ("-- sigsetup: handler %08x points to an invalid address\n",
		    handler_addr);
#endif
      return 1;
    }

#ifdef DEBUG
  debug_printf ("-- sigsetup: signo=%d (%s)\n",	signo, sys_siglist[signo]);
  debug_printf ("-- sigsetup: handler=%08x (%s)\n", handler_addr,
		extract_name ((unsigned int *) handler_addr, NULL));
#endif
  /* Zero if we are to execute the signal handler routine on a user
     supplied stack.  Non-zero if we are to use the normal system stack.  */
  int system_stack = (ss->signalstack.ss_flags & SA_DISABLE) != 0
		     || ss->signalstack.ss_size == 0
		     || ss->signalstack.ss_sp == 0;

  /* The user has requested to execute the signal on a sigstack
     but it can only be executed on the system stack. Return failure.  */
  if ((flags & SA_ONSTACK) && system_stack)
    {
#ifdef DEBUG
      debug_printf ("-- sigsetup: bad system stack. ss_size=%d, ss_sp=%d, ss_flags=%d\n",
		    ss->signalstack.ss_size,
		    ss->signalstack.ss_sp,
		    ss->signalstack.ss_flags);
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
      debug_printf ("-- sigsetup: will execute signal off a normal stack\n");
#endif
      /* For one argument signals, the last two arguments will be ignored.  */
      __unixlib_exec_sig (handler, signo, &ss->siginfo, &ss->ucontext);
#ifdef DEBUG
      debug_printf ("-- sigsetup: signal handler successfully executed\n");
#endif
      return 0;
    }

  /* Specify that the signal stack is currently in use.  */
  ss->signalstack.ss_flags |= SA_ONSTACK;

  /* If ss_size == -1, then this sigaltstack was the result of
     a BSD sigstack conversion. Run the BSD alternative signal
     executor.  */
  if ((int)ss->signalstack.ss_size == -1)
    {
      unsigned int low = (unsigned int) ss->signalstack.ss_sp;
      unsigned int high = low + 16384;
#ifdef DEBUG
      debug_printf ("-- sigsetup: will execute signal off a BSD stack\n");
#endif

      /* Perform address validation on the user supplied stack.
	 The stack must be within a read/write data area and not in
	 the first 32K page.  */
      if (low < 0x8000
	  || (low >= mem->robase && low < mem->rwbase)
	  || ! __valid_address ((void *) low, (void *) high))
	{
#ifdef DEBUG
	  debug_printf ("-- sigsetup: stack area (%08x - %08x) is invalid\n",
			low, high);
#endif
	  return 1;
	}
      __unixlib_exec_sigstack_bsd ((void *) low, handler, signo);
    }
  else
    {
      unsigned int low = (unsigned int) ss->signalstack.ss_sp;
      unsigned int high = low + ss->signalstack.ss_size;
#ifdef DEBUG
      debug_printf ("-- sigsetup: will execute signal off a POSIX stack\n");
#endif

      /* Perform address validation on the user supplied stack.  */
      if (low < 0x8000
	  || (low >= mem->robase && low < mem->rwbase)
	  || ! __valid_address ((void *) low, (void *) high))
	{
#ifdef DEBUG
	  debug_printf ("-- sigsetup: stack area (%08x - %08x) is invalid\n",
			low, high);
#endif
	  return 1;
	}

      __unixlib_exec_sigstack ((void *) low, ss->signalstack.ss_size,
			       handler, signo);
    }
  /* Say that the signal stack is no longer in use.  */
  ss->signalstack.ss_flags &= ~SA_ONSTACK;

#ifdef DEBUG
  debug_printf ("-- sigsetup: signal handler successfully executed\n");
#endif

  return 0;
}

static void
write_termination (int signo)
{
  fprintf (stderr, "\nTermination signal received: %s\n", sys_siglist[signo]);
}

/* Clang and GCC do not have compatible frame pointers.  */
#ifdef __clang__
#define FP_OFFSET (0)
#define LR_OFFSET (1)
#elif defined (__ARM_EABI__)
#define FP_OFFSET (-1)
#define LR_OFFSET (0)
#else
#define LR_OFFSET (-1)
#define FP_OFFSET (-3)
#endif

static void
__write_backtrace_thread (const unsigned int *fp)
{
  /* Running as USR26 or USR32 ?  */
  unsigned int is32bit;
  __asm__ volatile ("SUBS	%[is32bit], r0, r0\n\t" /* Set at least one status flag. */
		    "TEQ	pc, pc\n\t"
		    "MOVEQ	%[is32bit], #1\n\t"
		    : [is32bit] "=r" (is32bit)
		    : /* no inputs */
		    : "cc");
  const unsigned int pcmask = is32bit ? 0xfffffffcu : 0x03fffffcu;

  int features;
  if (_swix (OS_PlatformFeatures, _IN(0) | _OUT(0), 0, &features))
    features = 0;

  /* fp[0]  => pc
     fp[-1] => lr
     fp[-2] => sp
     fp[-3] => previous fp  */
  const unsigned int *oldfp = NULL;
  while (fp != NULL)
    {
      /* Check that FP is different.  */
      if (fp == oldfp)
	{
	  fprintf (stderr, "fp unchanged at %x\n", (unsigned int)fp);
	  break;
	}

      /* Validate FP address.  */
      if (!__valid_address (fp + FP_OFFSET, fp + FP_OFFSET + 1))
	{
	  fprintf (stderr, "Stack frame has gone out of bounds "
			   "with address %x\n", (unsigned int)(fp + FP_OFFSET));
	  break;
	}

#ifdef __ARM_EABI__
      const unsigned int * const lr = (unsigned int *)fp[LR_OFFSET];
      fprintf (stderr, "  (%8x) lr: %8x",
	       (unsigned int)fp, (unsigned int)lr);
#if PIC
      /* FIXME: extend this with source location when available.  */
      const char *lib = NULL;
      unsigned offset;
      _swix(SOM_Location,
	    _IN(0) | _OUTR(0,1), lr, &lib, &offset);
      if (lib)
	fprintf(stderr, " : %8X : %s\n", offset, lib);
      else
#endif
	fputc('\n', stderr);
#else
      /* Retrieve PC counter.
	 PC counter has been saved using STMxx ..., { ..., PC } so it can be
	 8 or 12 bytes away from the STMxx instruction depending on the ARM
	 processor flavor.  Correct it so it is aways 8 bytes away.  */
      const unsigned int *pc = (unsigned int *)(fp[0] & pcmask);
      if (!(features & 0x8))
	--pc;

      if (!__valid_address (pc, pc + 4))
	{
	  fprintf (stderr, "Invalid pc address %x\n", (unsigned int)pc);
	  break;
	}

      /* Retrieve lr.  */
      const unsigned int * const lr = (unsigned int *)(fp[-1] & pcmask);

      fprintf (stderr, "  (%8x) pc: %8x lr: %8x sp: %8x ",
	       (unsigned int)fp, (unsigned int)pc, (unsigned int)lr,
	       (unsigned int)fp[-2]);

      /* Retrieve function name.  */
      int cplusplus_name;
      const char *name = extract_name (pc, &cplusplus_name);
      fprintf (stderr, (cplusplus_name) ? " %s\n" : " %s()\n", name);
#endif
      oldfp = fp;
      fp = (const unsigned int *)fp[FP_OFFSET];
#ifndef __ARM_EABI__
      if (__ul_callbackfp != NULL && fp == __ul_callbackfp)
	{
	  /* At &oldfp[1] = cpsr, a1-a4, v1-v6, sl, fp, ip, sp, lr, pc */
	  fprintf (stderr, "\n  Register dump at %08x:\n",
		   (unsigned int) &oldfp[1]);

	  if (!__valid_address (oldfp + 1, oldfp + 18))
	    fputs ("\n    [bad register dump address]\n", stderr);
	  else
	    {
	      const char rnames[] = "a1a2a3a4v1v2v3v4v5v6slfpipsplrpc";
	      for (int reg = 0; reg < 16; reg++)
		{
		  if ((reg & 0x3) == 0)
		    fputs ("\n   ", stderr);
		  
		  fprintf (stderr, " %c%c: %8x",
			   rnames[2*reg + 0], rnames[2*reg + 1], oldfp[reg + 2]);
		}

	      if (is32bit)
		fprintf (stderr, "\n    cpsr: %8x\n", oldfp[1]);
	      else
		{
		  const char * const pmode[4] = { "USR", "FIQ", "IRQ", "SVC" };
		  fprintf (stderr, "\n    Mode %s, flags set: %c%c%c%c%c%c\n",
			   pmode[oldfp[15 + 2] & 3],
			   (oldfp[15 + 2] & (1<<31)) ? 'N' : 'n',
			   (oldfp[15 + 2] & (1<<30)) ? 'Z' : 'z',
			   (oldfp[15 + 2] & (1<<29)) ? 'C' : 'c',
			   (oldfp[15 + 2] & (1<<28)) ? 'V' : 'v',
			   (oldfp[15 + 2] & (1<<27)) ? 'I' : 'i',
			   (oldfp[15 + 2] & (1<<26)) ? 'F' : 'f');
		}

	      pc = (unsigned int *) (oldfp[17] & pcmask);

	      /* Try LR if PC invalid (e.g. with a prefetch abort).  */
	      if (pc < (unsigned int *)0x8000 || !__valid_address (pc - 5, pc + 4))
		pc = (unsigned int *) (oldfp[16] & pcmask);

	      if (pc >= (unsigned int *)0x8000 && __valid_address (pc - 5, pc + 4))
		{
		  for (unsigned int *diss = pc - 5; diss < pc + 4; diss++)
		    {
		      const char *ins;
		      int length;
		      _swix (Debugger_Disassemble, _INR(0,1) | _OUTR(1,2),
			     *diss, diss, &ins, &length);

		      const unsigned char c[4] =
			{
			  (*diss >>  0) & 0xFF,
			  (*diss >>  8) & 0xFF,
			  (*diss >> 16) & 0xFF,
			  (*diss >> 24)
			};
		      fprintf (stderr, "\n  %08x : %c%c%c%c : %08x : ",
			       (unsigned int) diss,
			       (c[0] >= ' ' && c[0] != 127) ? c[0] : '.',
			       (c[1] >= ' ' && c[1] != 127) ? c[1] : '.',
			       (c[2] >= ' ' && c[2] != 127) ? c[2] : '.',
			       (c[3] >= ' ' && c[3] != 127) ? c[3] : '.',
			       *diss);
		      fwrite (ins, length, 1, stderr);
		    }
		}
	      else
		fputs ("\n  [Disassembly not available]", stderr);
	    }

	  fputs ("\n\n", stderr);
	}
#endif
    }

  fputc ('\n', stderr);
}


void
__write_backtrace (int signo)
{
#ifdef __ARM_EABI__
  register const unsigned int *fp = __builtin_frame_address(0);
#else
  register const unsigned int *fp __asm ("fp");
#endif

  PTHREAD_UNSAFE

  if (signo != 0)
    fprintf (stderr, "\nFatal signal received: %s\n\n", strsignal (signo));

#ifdef __ARM_EABI__
  {
    int abort_error_code;
    if (_swix(ARMEABISupport_AbortOp, _IN(0)|_OUT(0),
				      ARMEABISUPPORT_ABORTOP_READ_ERROR,
				      &abort_error_code) == NULL)
      {
        if (abort_error_code > 0 && abort_error_code < abort_ERROR_MAX)
	  fputs(abort_msg[abort_error_code], stderr);
	else if (abort_error_code)
	  fprintf(stderr, abort_msg[0], abort_error_code);
      }
  }
#endif

#ifndef __SOFTFP__
  if (signo == SIGFPE)
    {
#ifdef __VFP_FP__
      for (int i = 0; i < __ul_global.vfp_regcount; i++)
        {
          fprintf(stderr, "  d%02d: %016" PRIx64 "%c", i, __ul_fp_registers.d[i],
		  ((i == __ul_global.vfp_regcount - 1) || ((i & 3) == 3)) ? '\n': '\0');
        }
      fprintf(stderr, "  fpscr: %x\n", __ul_fp_registers.fpscr);
#else
      fprintf(stderr, "  f0: %f  f1: %f  f2: %f  f3: %f\n"
		      "  f4: %f  f5: %f  f6: %f  f7: %f\n  fpsr: %x\n",
		      __ul_fp_registers.f[0], __ul_fp_registers.f[1],
		      __ul_fp_registers.f[2], __ul_fp_registers.f[3],
		      __ul_fp_registers.f[4], __ul_fp_registers.f[5],
		      __ul_fp_registers.f[5], __ul_fp_registers.f[7],
		      __ul_fp_registers.fpsr);
#endif
    }
#endif

  /* Dump first the details of the current thread.  */
  fprintf (stderr, "Stack backtrace:\n\nRunning thread %p (%s)\n",
	   __pthread_running_thread, __pthread_running_thread->name);
  __write_backtrace_thread (fp);

  /* And then the other suspended threads if any.  */
  for (pthread_t th = __pthread_thread_list; th != NULL; th = th->next)
    {
      if (th == __pthread_running_thread)
        continue;

      fprintf (stderr, "\nThread %p (%s)\n", th, th->name);
#ifdef __clang__
      const unsigned int fakestackframe[] =
        {
          (unsigned int)th->saved_context->r[11],
          (unsigned int)th->saved_context->r[14]
        };
      __write_backtrace_thread (&fakestackframe[0]);
#elif defined (__ARM_EABI__)
      const unsigned int fakestackframe[] =
        {
          (unsigned int)th->saved_context->r[11],
          (unsigned int)th->saved_context->r[14]
        };
      __write_backtrace_thread (&fakestackframe[1]);
#else
      const unsigned int fakestackframe[] =
        {
          (unsigned int)th->saved_context->r[11],
          (unsigned int)th->saved_context->r[13],
          (unsigned int)th->saved_context->r[14],
          (unsigned int)th->saved_context->r[15]
        };
      __write_backtrace_thread (&fakestackframe[3]);
#endif
    }
}

static void __attribute__ ((noreturn))
__signal_restart (struct sigcontext *ctx)
{
  __asm__ volatile ("LDMIA %[base], {r0-r15}\n\t"
		    : /* No Outputs */
		    : [base] "r" (ctx));
  abort (); /* Never reached.  */
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
  struct ul_global *gbl = &__ul_global;
  struct __sul_process *sulproc = gbl->sulproc;
  enum
  {
    stop, ignore, core, term, handle
  } act;

  /* 0 is the special signo used for posting any pending signals.  */
  if (signo == 0)
    goto post_pending;

post_signal:
#ifdef DEBUG
  debug_printf ("-- post_signal: sigstate=%08x, signo %d (%s)\n",
		ss, signo, sys_siglist[signo]);
#endif

  /* Increment the number of signals this process has received.  */
  __u->usage.ru_nsignals++;

  __sighandler_t handler = SIG_DFL;
  const sigset_t signal_mask = sigmask (signo);

  if (handler != SIG_DFL)
    /* Run the preemption-provided handler.  */
    act = handle;
  else
    {
      /* Do normal handling.  */
      handler = ss->actions[signo].sa_handler;
#ifdef DEBUG
      {
	const char *name;
	if (handler == SIG_ERR)
	  name = "SIG_ERR";
	else if (handler == SIG_DFL)
	  name = "SIG_DFL";
	else if (handler == SIG_IGN)
	  name = "SIG_IGN";
	else
	  name = extract_name ((const unsigned int *) (unsigned int) handler, NULL);
	debug_printf ("-- post_signal: handler=%08x (%s)\n",
		      handler, name);
      }
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
	      if (sulproc->pgrp == sulproc->pid)
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
	  sulproc->status.stopped = 0;
	  sulproc->status.reported = 0;
	}
    }

#ifdef DEBUG
  {
    const char * const s[] = { "stop", "ignore", "core", "term", "handle" };
    debug_printf ("-- post_signal: action taken = %s\n", s[act]);
  }
#endif

  if (sulproc->ppid == 1 && act == stop
      && (signal_mask & (sigmask (SIGTTIN)
			 | sigmask (SIGTTOU)
			 | sigmask (SIGTSTP))) != 0)
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
      debug_printf ("-- post_signal: stop\n");
#endif
      /* Stop all our threads, and mark ourselves stopped.  */
      sulproc->status.stopped = 1;
      /* Wake up sigsuspend. */
      __u->sleeping = 0; /* inline version of sigwakeup() */
      break;

    case ignore:
#ifdef DEBUG
      debug_printf ("-- post_signal: ignore\n");
#endif
      /* Nobody cares about this signal.  */
      break;

    case term:			/* Time to die.  */
    case core:			/* and leave a rotting corpse.  */
death:
      /* Stop all other threads in our task.  */
      /* No more user instructions will be executed. */
      {
	int status = W_EXITCODE (0, signo);

	/* Do a core dump if desired. Only set the wait status bit saying
	   we in fact dumped core if the operation was actually successful.  */
#ifdef DEBUG
	debug_printf ("-- post_signal: term/core\n");
#endif

	if (__get_taskhandle ()
	    && !gbl->taskwindow && isatty (fileno (stderr)))
	  {
	    int regs[10];
	    regs[0] = (int)__u->argv[0];
	    __os_swi (Wimp_CommandWindow, regs);
	  }

	if (signo == SIGOSERROR && __pthread_running_thread->errbuf_valid)
	  fprintf(stderr, "\nError 0x%x: %s\n  pc: %x\n",
			  __ul_errbuf.errnum, __ul_errbuf.errmess,
			  (int)(int *)__ul_errbuf.pc);

	if (act == term)
	  write_termination (signo);
	else if (act == core)
	  {
	    __write_backtrace (signo);
	    const _kernel_oserror *roerr;
	    if ((roerr = __unixlib_write_coredump (NULL)) != NULL)
	      fprintf (stderr, "Failed to coredump: %s\n", roerr->errmess);
	    status |= WCOREFLAG;
	  }

	/* Die, returning information about how we died.  */
	_exit (status);
	/* Never reached.  */
	break;
      }

    case handle:
      /* Call a handler for this signal.  */
      {
#ifdef DEBUG
	debug_printf ("-- post_signal: handle\n");
#endif
	/* We're going to handle this signal now, so remove it from
	   the pending list.  */
	sigdelset (&ss->pending, signo);

	/* Block SIGNO and requested signals while running the handler.  */
	sigset_t blocked = ss->blocked;
	ss->blocked |= signal_mask | ss->actions[signo].sa_mask;
	int flags = ss->actions[signo].sa_flags;
	/* Re-instate the default signal handler.  We do this before executing
	   the signal handler because a new handler might be setup whilst
	   executing the signal handler.  */
	ss->actions[signo].sa_handler = SIG_DFL;
	ss->actions[signo].sa_flags = SA_RESTART;
	sigemptyset (&ss->actions[signo].sa_mask);

	memset (&ss->ucontext, 0, sizeof (struct sigcontext));
	if (__signal_have_saved_regs (signo))
	  {
	    /* For those signals that support it, copy the registers from the save
	       block.  */
	    memcpy (&ss->ucontext.arm_r0, __ul_global.pthread_callevery_rma->callback_regs, 4 * 17); /* r0-r15 + cpsr */
	    ss->ucontext.fault_address = ss->ucontext.arm_pc;
	  }

	/* Set up the siginfo structure.  */
	memset (&ss->siginfo, 0, sizeof (siginfo_t));
	ss->siginfo.si_signo = signo;
	ss->siginfo.si_pid = sulproc->pid;
	ss->siginfo.si_addr = (void *)ss->ucontext.fault_address;

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

	/* If the signal handler changed the PC of the saved registers, then
	   restore all registers and pass control to the new location. */
	if (__signal_have_saved_regs (signo) &&
	    ss->ucontext.arm_pc != ss->ucontext.fault_address)
	  {
	    gbl->executing_signalhandler = 0;
	    if (gbl->pthread_system_running)
	      __pthread_enable_ints();
	    __signal_restart (&ss->ucontext);
	  }
	break;
      }
    }

  /* Deliver pending signals.  */

post_pending:
#ifdef DEBUG
  debug_printf ("-- post_signal: Deliver pending signals."
		" Pending %08x, blocked %08x\n",
		ss->pending, ss->blocked);
#endif
  if (!__u->stopped)
    {
       const sigset_t pending = ss->pending & ~ss->blocked;
       if (pending)
	{
	  for (signo = 1; signo < NSIG; ++signo)
	    if (sigismember (&pending, signo))
	      {
		sigdelset (&ss->pending, signo);
		goto post_signal;
	      }
	}
    }

  ss->currently_handling = 0;
#ifdef DEBUG
  debug_printf ("-- post_signal: pending signals delivered\n");
#endif
  /* No more signals pending.  */
  __u->sleeping = 0; /* inline version of sigwakeup (); */
}

void
__unixlib_raise_signal (struct unixlib_sigstate *ss, int signo)
{
  PTHREAD_UNSAFE

#ifdef DEBUG
  __os_nl ();
  if (signo == 0)
    debug_printf ("-- __ul_raise_signal: deliver pending signals\n");
  else
    debug_printf ("-- __ul_raise_signal: sigtate %08x, signo %d (%s)\n",
		  ss, signo, sys_siglist[signo]);
#endif

  /* When signals are raised, we could be in any state imaginable.  Be
     extra careful and verify the basic UnixLib ingredients before
     delivery.

     It is unlikely that we can recover from errors here, so print an
     error message and die.  */
  const char *err;
  if (__u == NULL)
    {
      err = "UnixLib process structure is non-existent.";
      goto error;
    }

  if (! __valid_address (&__u[0], &__u[1]))
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
    sigaddset (&ss->pending, signo);

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
      debug_printf ("-- __ul_raise_signal: signal %d (%s) is pending for delivery\n",
		    signo, sys_siglist[signo]);
#endif
    }

#ifdef DEBUG
  debug_printf ("-- __ul_raise_signal: returning\n");
#endif

  return;

error:
  __os_nl ();
  __os_print (err);
  __os_print ("  Exiting.");
  __os_nl ();
  _exit (W_EXITCODE (0, signo));
}
