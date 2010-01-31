/* signal.h standard header for the RISC OS SharedCLibrary.
   Copyright (c) 1997-2005 Nick Burrett
   All rights reserved.
 
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   3. The name of the author may not be used to endorse or promote products
      derived from this software without specific prior written permission.
 
   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

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
