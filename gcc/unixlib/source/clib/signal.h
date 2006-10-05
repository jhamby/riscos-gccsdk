/*
 * ANSI, POSIX and BSD signal handling.
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef __SIGNAL_H
#define __SIGNAL_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif
#ifndef __STDDEF_H
#include <stddef.h>
#endif

#define __need_pthread_t
#include <pthread.h>

__BEGIN_DECLS

#include <bits/signum.h>

/* Type of a signal handler.  */
typedef void (*__sighandler_t)(int);
#ifdef __USE_GNU
typedef __sighandler_t sighandler_t;
#endif

extern int sys_nsig;		/* = NSIG */
extern const char * const sys_siglist[NSIG];	/* signal messages */


extern __sighandler_t signal (int __sig, __sighandler_t __handler) __THROW;

/* Send signal 'sig' to process number 'pid'.  If pid is zero,
   send sig to all processes in the current process's process group.
   If pid is < -1, send sig to all processes in process group -pid.  */
extern int kill (__pid_t __pid, int __sig) __THROW;

/* Send signal 'sig' to all processes in process group 'pgrp'.
   If pgrp is zero, send sig to all processes in the current
   process's process group.  */
extern int killpg (__pid_t __pgrp, int __sig) __THROW;

/* Raise signal sig.  */
extern int raise (int __sig) __THROW;

/* Print a message describing the meaning of the given signal number.  */
extern void psignal (int __sig, const char *__s) __THROW;


/* BSD signal functions.  */

/* Block signals in mask, returning the old mask.  */
extern int sigblock (int __mask) __THROW;

/* Set the mask of blocked signals to mask, return the old mask.  */
extern int sigsetmask (int __mask) __THROW;

/* Set the mask of blocked signals to mask, wait for a signal
   to arrive, and then restore the mask.

   This function is a cancellation point.  */
extern int sigpause (int __mask);

#define sig_atomic_t __sig_atomic_t

/* POSIX signal functions.  */

/* Don't rely on sigset_t remaining an unsigned long. It's usage
   could soon change into the structure form below. Use the
   signal functions below to manipulate its value.  */

#define sigset_t __sigset_t

#if 0
typedef struct __usigset
{
  unsigned int sig[NSIG] : 1;
} __sigset_t;
#endif

/* Structure describing the action to be taken when a signal arrives.  */
struct sigaction
  {
    /* Signal handler.  */
    __sighandler_t sa_handler;
    /* Additional set of signals to be blocked.  */
    __sigset_t sa_mask;
    /* Special flags.  */
    int sa_flags;
  };

/* Bits in sa_flags.  */

/* Take signal on signal stack.  */
#define	SA_ONSTACK	0x1
/* Don't restart syscall on signal return.  */
#define	SA_RESTART	0x2
/* Disable alternate signal stack.  */
#define	SA_DISABLE	0x4
/* Don't send SIGCHLD when children stop.  */
#define	SA_NOCLDSTOP	0x8


/* Values for the HOW argument to `sigprocmask'.  */

/* Block signals.  */
#define	SIG_BLOCK	1
/* Unblock signals.  */
#define	SIG_UNBLOCK	2
/* Set the set of blocked signals.  */
#define	SIG_SETMASK	3

/* Currently in an optimised state but this could soon change.  */

/* Clear all signals from set.  */
extern int sigemptyset (__sigset_t *__set) __THROW __nonnull ((1));
#define sigemptyset(set) ((*(set) = (__sigset_t) 0), 0)

/* Set all signals in set.  */
extern int sigfillset (__sigset_t *__set) __THROW __nonnull ((1));
#define sigfillset(set) ((*(set) = ~(__sigset_t) 0), 0)

extern __sigset_t sigmask (int __sig) __THROW;
#define sigmask(sig) (((__sigset_t) 1) << ((sig) - 1))


/* Add signo to set.  */
extern int sigaddset (__sigset_t *__set, int __signo) __THROW __nonnull ((1));

/* Remove signo from set.  */
extern int sigdelset (__sigset_t *__set, int __signo) __THROW __nonnull ((1));

/* Remove 1 if signo is in set, 0 if not.  */
extern int sigismember (const __sigset_t *__set, int __signo)
     __THROW __nonnull ((1));

#ifdef __UNIXLIB_INTERNALS
/* Inline versions for UnixLib library only. These are subject to change.  */

/* Add signo to set.  */
#define sigaddset(set,signo) ((*(set) |= sigmask (signo)), 0)

/* Remove signo from set.  */
#define sigdelset(set,signo) ((*(set) &= ~sigmask (signo)), 0)

/* Remove 1 if signo is in set, 0 if not.  */
#define sigismember(set,signo) ((*(set) & sigmask (signo)) ? 1 : 0)

#endif  /* __UNIXLIB_INTERNALS */

/* Get and/or change the set of blocked signals.  */
extern int sigprocmask (int __how, const __sigset_t *__restrict __set,
			__sigset_t *__oldset) __THROW;

/* Change the set of blocked signals,
   wait until a signal arrives, and restore the set of blocked signals.
   This function is a cancellation point.  */
extern int sigsuspend (const __sigset_t *__set);

/* Get and/or set the action for signal sig.  */
extern int sigaction (int __sig, const struct sigaction *__restrict __act,
		      struct sigaction *__oldact) __THROW;

/* Put in set all signals that are blocked and waiting to be delivered.  */
extern int sigpending (__sigset_t *__set) __THROW;

/* BSD signal handling functionality.  */
#if 0
struct sigvec
{
  /* Signal handler.  */
  sighandler_t sv_handler;
  /* Mask of signals to be blocked.  */
  int sv_mask;
  /* Flags.  */
  int sv_flags;
/* 4.2 BSD compatibility.  */
#define sv_onstack sv_flags
};

/* Bits in sv_flags.  */

/* Take the signal on the signal stack. */
#define SV_ONSTACK 1
/* Do not restart system calls.  */
#define SV_INTERRUPT 2
/* Reset handler to SIG_DFL on receipt.  */
#define SV_RESETHAND 4

extern int sigvec (int __sig, const struct sigvec *__vec,
		   struct sigvec *__ovec) __THROW;
#endif

/* If interrupt is nonzero, make signal sig interrupt system calls
   (causing them to fail with EINTR); if interrupt is zero, make
   system calls be restarted after signal sig.  */
extern int siginterrupt (int __sig, int __interrupt) __THROW;

/* Signal stack structure (BSD style).  */
struct sigstack
  {
    /* Signal stack pointer.  */
    void *ss_sp;
    /* Non zero if executing on this stack.  */
    int ss_onstack;
  };

/* Run signal handlers on the stack specified by 'ss' (if not NULL).
   The old status is returned in oss (if not NULL).  */
extern int sigstack (const struct sigstack *__ss,
		     struct sigstack *__oss) __THROW;

/* Signal stack structure (POSIX alternative interface).  */
struct sigaltstack
  {
    /* Pointer to stack base.  */
    void *ss_sp;
    /* Size of stack.  */
    size_t ss_size;
    /* sigaltstack flags.  */
    int ss_flags;
  };

extern int sigaltstack (const struct sigaltstack *__ss,
			struct sigaltstack *__oss) __THROW;

/* Signal stack constants.  */

/* The recommended signal stack size for a signal handler.  */
#define SIGSTKSZ 4096

/* The absolute safe minimum signal stack size
   for running signal handlers in.  */

#define MINSIGSTKSZ 2048

/* Send a signal to a specific thread */
extern int pthread_kill (pthread_t thread, int sig) __THROW;

/* Set the signal mask for a thread */
extern int pthread_sigmask (int how, const sigset_t *set,
			    sigset_t *oset) __THROW;

/* Suspend thread until delivery of a signal.
   This function is a cancellation point.  */
extern int sigwait (const sigset_t *__restrict set, int *sig);

__END_DECLS

#endif
