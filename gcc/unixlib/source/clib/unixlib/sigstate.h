/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/unixlib/h/sigstate,v $
 * $Date: 2000/06/03 12:22:55 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* This is an internal UnixLib header file for implementing the signal
   handlers.  These functions cannot be used in a user program.  */

#ifndef __UNIXLIB_SIGSTATE_H
#define __UNIXLIB_SIGSTATE_H 1

#ifndef __SIGNAL_H
#include <signal.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define __SIG_INVALID_P(sig) ((unsigned int)sig >= NSIG)

/* Signal state for each thread.  */

struct unixlib_sigstate
  {
    /* The blocked signals for this process.  */
    sigset_t blocked;
    /* The pending signals for this process.  */
    sigset_t pending;
    /* The defined signal handlers for this process.  */
    struct sigaction actions[NSIG];
    struct sigaltstack signalstack;

    /* If `suspended' is set when this process gets a signal,
       the signal thread sends an empty message to it.  */
    int suspended;

    /* If 'currently_handling' is set, then the process is currently
       executing a signal handler. Any raised signals will be pended and
       executed after the current signal has finished executing.  */
    int currently_handling;
  };

/* Initialize the signal code.  */
extern void __unixlib_sig_init (void);

/* Raise a signal as described by __signo on the process whose sigstate
   __ss points to. If __ss is null, this will affect the calling process.  */
extern void __unixlib_raise_signal (struct unixlib_sigstate *__ss,
       	    			   int __signo);


/* Make the thread described by __ss take the signal described by __signo.  */
extern void __unixlib_internal_post_signal (struct unixlib_sigstate *__ss,
					int __signo);

/* Set up __ss to handle signal __signo by running __handler. The
   handler is passed __signo.  */
extern int __unixlib_setup_sighandler (struct unixlib_sigstate *__ss,
			__sighandler_t __handler, int __signo, int __flags);

/* Function run for SIGINFO when its action is SIG_DFL and the current
   process is the session leader.  */
extern void __unixlib_siginfo_handler (int);

/* Actual signal execution functions. Depends on whether we are
   using sigstack, sigaltstack or not.  */
extern void __unixlib_exec_sig (__sighandler_t, int);
extern void __unixlib_exec_sigstack_bsd (void *__sp, __sighandler_t, int);
extern void __unixlib_exec_sigstack (void *__sp, int size, __sighandler_t, int);
extern void __unixlib_default_sigaction (struct unixlib_sigstate *);

/* Writes a friendly error message depending on whether the signal
   produces a core dump or just terminates.  */
extern int __write_corefile (int __signo);
extern void __write_termination (int __signo);

/* Signal handler support functions.  */

/* Undefined instruction handler.  */
extern void __h_sigill (void);

/* Address exception handler.  */
extern void __h_sigbus (void);

/* Prefetch abort handler.  */
extern void __h_sigsegv0 (void);

/* Data abort handler.  */
extern void __h_sigsegv1 (void);

/* Floating point exception handler.  */
extern void __h_sigfpe (void);

/* Error handler. */
extern void __h_error (void);

/* Escape handler.  */
extern void __h_sigint (void);

/* Event handler.  */
extern void __h_event (void);

/* Unused SWI handler.  */
extern void __h_sigsys (void);

/* UpCall handler.  */
extern void __h_upcall (void);

/* SIGALRM handler.  */
extern void __h_sigalrm (void);
extern void __h_sigalrm_init (void);

/* SIGVTALRM handler.  */
extern void __h_sigvtalrm (void);
extern void __h_sigvtalrm_init (void);

/* SIGPROF handler.  */
extern void __h_sigprof (void);
extern void __h_sigprof_init (void);

/* CallBack handler.  */
extern void __h_cback (void);

/* Exit handler - calls _exit().  */
extern void __h_exit (void);

/* Callback handler register buffer.  */
extern unsigned int __cbreg[16];

/* Error handler string buffer.  */
extern char *__h_errbuf;

/* Alarm semaphores.  */
extern int __h_sigprof_sema, __h_sigvtalrm_sema, __h_sigalrm_sema;

/* Wake a process up from sleeping.  */
extern void sigwakeup (void);

/* Details for stack backtraces and core dumps.  */

/* Generate a stack backtrace, __fp is the value of the
   current frame pointer. */
extern void __backtrace (const unsigned int *__fp);

/* Generate a core dump, from the current frame pointer.  */
extern void __core (void);

#ifdef __cplusplus
}
#endif

#endif
