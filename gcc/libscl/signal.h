/* signal.h

   For use with the GNU compilers and the SharedCLibrary.
   (c) Copyright 1997, Nick Burrett.  */

#ifndef __SIGNAL_H
#define __SIGNAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Reading and writing this data type is guaranteed to
   happen in a single instruction, so there's no way for
   a handler to run in the middle of an access.  */
typedef int sig_atomic_t;

extern void __default_signal_handler (int);
extern void __error_signal_marker (int);
extern void __ignore_signal_handler (int);

/* Specifies the default action for the particular signal.  */
#define SIG_DFL &__default_signal_handler

/* Used to flag error return from signal.  */
#define SIG_ERR &__error_signal_marker

/* Specifies that the signal should be ignored.  */
#define SIG_IGN &__ignore_signal_handler

/* Defined signals.  */

/* Indicates an error detected by the program itself and reported
   by calling abort().  */
#define SIGABRT 1

/* Fatal arithmetic error i.e. all arithmetic errors, including
   division by zero and overflow.  */
#define SIGFPE 2

/* Illegal instruction.  */
#define SIGILL 3

/* The program interrupt signal is sent when the user types the
   INTR character, usually Escape.  */
#define SIGINT 4

/* Signal generated when a program tries to read/write outside the
   memory that is allocated to it, or to write memory that can only
   be read.  */
#define SIGSEGV 5

/* Generic signal used to cause program termination.  */
#define SIGTERM 6

/* Stack overflow.  */
#define SIGSTAK 7

/* User definable signal.  */
#define SIGUSR1 8

/* User definable signal.  */
#define SIGUSR2 9

/* Operating system error.  */
#define SIGOSERROR 10

typedef void Handler (int);

/* Define the function 'action' to be called when 'signum' is raised.  */
extern Handler *signal (int __signum, Handler *__action);

/* Sends the signal 'sig' to the executing program. Returns zero on
   success, non-zero on failure.  */
extern int raise (int __sig);

#ifdef __cplusplus
}
#endif

#endif
