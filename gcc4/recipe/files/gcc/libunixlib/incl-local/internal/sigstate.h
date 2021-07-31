/* This is an internal UnixLib header file for implementing the signal
   handlers.  These functions cannot be used in a user program.
   Copyright 2000-2010 UnixLib Developers.  */

#ifdef __TARGET_SCL__
#  error "SCL has no sigstate support"
#endif

#ifndef __INTERNAL_SIGSTATE_H
#define __INTERNAL_SIGSTATE_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __SIGNAL_H
#include <signal.h>
#endif

#ifndef _UCONTEXT_H
#include <ucontext.h>
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

    /* The state of the current signal being handled. This is only defined
       when the signal was established by sigaction() and not signal(). Not
       all members are used.  */
    siginfo_t siginfo;

    /* Snapshot of the registers at the time the signal was triggered.  */
    struct sigcontext ucontext;
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
extern void __unixlib_exec_sig (__sighandler_t, int, siginfo_t *, void *);
extern void __unixlib_exec_sigstack_bsd (void *__sp, __sighandler_t, int);
extern void __unixlib_exec_sigstack (void *__sp, int size, __sighandler_t, int);
extern void __unixlib_default_sigaction (struct unixlib_sigstate *);

/* Returns non-zero value when address range __lower - __upper (excl) is
   a valid address range.  */
extern int __valid_address (const void *__lower, const void *__upper);

/* Demangle C++ symbols (only when libstdc++ was available during
   linktime).  Is a wrapper around __cxa_demangle() and follows the
   same API.  When libstd++ wasn't available during linking, the returned
   value equals '__mangled_name'.  */
extern char *__unixlib_cxa_demangle (const char *__mangled_name,
				     char *__output_buffer, size_t *__length,
				     int *__status);

/* SIGALRM handler.  */
extern void __h_sigalrm_init (void);

/* SIGVTALRM handler.  */
extern void __h_sigvtalrm_init (void);

/* SIGPROF handler.  */
extern void __h_sigprof_init (void);

/* Alarm semaphores.  */
extern int __h_sigprof_sema, __h_sigvtalrm_sema, __h_sigalrm_sema;

/* When non-NULL, the fp at the moment __h_cback_common is executed.  */
extern unsigned int * __ul_callbackfp;

/* Defines the coredump directory when __unixlib_write_coredump doesn't
   have a non-NULL directory specfied.  */
extern void __init_coredump (const char *dir);

__END_DECLS

#endif
