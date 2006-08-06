/* Signal number definitions.
   Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers.  */

#ifndef __BITS_SIGNUM_H
#define __BITS_SIGNUM_H

/* Keep the following in sync with asm_dec.s. */
/* Hangup (POSIX).  */
#define SIGHUP		1
/* Interrupt (ANSI).  */
#define SIGINT		2
/* Quit (POSIX).  */
#define SIGQUIT		3
/* Illegal instruction (ANSI).  */
#define SIGILL		4
/* Abort (ANSI).  */
#define SIGABRT		SIGIOT
/* Trace trap (POSIX).  */
#define SIGTRAP		5
/* IOT trap (4.2 BSD).  */
#define SIGIOT		6
/* EMT trap (4.2 BSD).  */
#define SIGEMT		7
/* Stack extension failed (RISC OS Acorn C compatibility) */
#define SIGSTAK		SIGEMT
/* Floating-point exception (ANSI).  */
#define SIGFPE		8
/* Kill, unblockable (POSIX).  */
#define SIGKILL		9
/* Bus error (4.2 BSD).  */
#define SIGBUS		10
/* Segmentation violation (ANSI).  */
#define SIGSEGV		11
/* Bad argument to system call (4.2 BSD).  */
#define SIGSYS		12
/* Broken pipe (POSIX).  */
#define SIGPIPE		13
/* Alarm clock (POSIX).  */
#define SIGALRM		14
/* Termination (ANSI).  */
#define SIGTERM		15
/* Urgent, or out-of-band data arrives on a socket (4.2 BSD). */
#define SIGURG		16
/* Stop, unblockable (POSIX).  */
#define SIGSTOP		17
/* Keyboard stop, Ctrl-Z (POSIX).  */
#define SIGTSTP		18
/* Continue (POSIX).  */
#define SIGCONT		19
/* Child process has terminated or stopped (POSIX).  */
#define SIGCHLD		20
/* Obsolete name for SIGCHLD (System V).  */
#define SIGCLD		SIGCHLD
/* Background read from tty (POSIX).  */
#define SIGTTIN		21
/* Backgroung write to tty (POSIX).  */
#define SIGTTOU		22
/* File descriptor is ready to perform input or output (4.2 BSD).  */
#define SIGIO		23
/* Similar to SIGIO (System V). */
#define SIGPOLL		SIGIO
/* CPU time limit exceeded (4.2 BSD).  */
#define SIGXCPU		24
/* File size limit exceeded (4.2 BSD).  */
#define SIGXFSZ		25
/* Virtual alarm clock (4.2 BSD).  */
#define SIGVTALRM	26
/* Profiling alarm clock (4.2 BSD).  */
#define SIGPROF		27
/* Window size change (4.3 BSD)  */
#define SIGWINCH	28
/* Information request (4.4 BSD). */
#define SIGINFO		29
/* User-defined signal 1 (POSIX).  */
#define SIGUSR1		30
/* User-defined signal 2 (POSIX).  */
#define SIGUSR2		31
/* Resource lost.  */
#define SIGLOST		32
/* RISC OS error.  */
#define SIGOSERROR	33

/* Total number of signals defined. This should be one greater than
   the largest defined signal number.  */
#define _NSIG 34
#define NSIG _NSIG

/* Fake signal functions, based on the BSD versions.  */

/* Error return.  */
#define SIG_ERR ((__sighandler_t) -1)

/* Default action.  */
#define SIG_DFL ((__sighandler_t) 0)

/* Ignore signal.  */
#define SIG_IGN ((__sighandler_t) 1)


#endif /* __BITS_SIGNAL_H */
