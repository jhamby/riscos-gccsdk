/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/unixlib/sigstate.h,v $
 * $Date: 2002/09/24 21:02:37 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* This is an internal UnixLib header file for implementing the signal
   handlers.  These functions cannot be used in a user program.  */

#ifndef __UNIXLIB_SIGSTATE_H
#define __UNIXLIB_SIGSTATE_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

#ifndef __SIGNAL_H
#include <signal.h>
#endif

__BEGIN_DECLS

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

/* Raise a signal as described by __signo on the process whose sigstate
   __ss points to. If __ss is null, this will affect the calling process.  */
extern void __unixlib_raise_signal (struct unixlib_sigstate *__ss,
       	    			   int __signo);

/* Function run for SIGINFO when its action is SIG_DFL and the current
   process is the session leader.  */
extern void __unixlib_siginfo_handler (int);

/* Actual signal execution functions. Depends on whether we are
   using sigstack, sigaltstack or not.  */
extern void __unixlib_exec_sig (__sighandler_t, int);
extern void __unixlib_exec_sigstack_bsd (void *__sp, __sighandler_t, int);
extern void __unixlib_exec_sigstack (void *__sp, int size, __sighandler_t, int);
extern void __unixlib_default_sigaction (struct unixlib_sigstate *);

/* Helper C function for signal handler */
void __write_unrecoverable(const char *errmess);

/* SIGALRM handler.  */
extern void __h_sigalrm_init (void);

/* SIGVTALRM handler.  */
extern void __h_sigvtalrm_init (void);

/* SIGPROF handler.  */
extern void __h_sigprof_init (void);

/* Alarm semaphores.  */
extern int __h_sigprof_sema, __h_sigvtalrm_sema, __h_sigalrm_sema;

/* Details for stack backtraces and core dumps.  */

/* Generate a stack backtrace, __fp is the value of the
   current frame pointer. */
extern void __backtrace (const unsigned int *__fp);

/* Generate a core dump, from the current frame pointer.  */
extern void __core (void);

__END_DECLS

#endif
