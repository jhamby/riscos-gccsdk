/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sleep.c,v $
 * $Date: 2003/04/28 21:04:36 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sleep.c,v 1.6 2003/04/28 21:04:36 alex Exp $";
#endif

/* Copyright (C) 1991, 1992, 1993, 1996, 1997 Free Software Foundation, Inc.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <unixlib/unix.h>
#include <pthread.h>

#define USECS_PER_CLOCK (1000000 / CLOCKS_PER_SEC)

/* #define DEBUG */

#ifdef DEBUG
#include <unixlib/os.h>
#endif

/* SIGALRM signal handler for `sleep'.  This does nothing but return,
   but SIG_IGN isn't supposed to break `pause'.  */
static void
sleep_handler (int sig)
{
  sig = sig;
  return;
}

/* Make the process sleep for CLOCKTICKS clock ticks, or until a signal
   arrives and is not ignored.  The function returns the number of clock
   ticks less than CLOCKTICKS which it actually slept (zero if it slept
   the full time).
   If a signal handler does a `longjmp' or modifies the handling of the
   SIGALRM signal while inside `sleep' call, the handling of the SIGALRM
   signal afterwards is undefined.  There is no return value to indicate
   error, but if `sleep' returns SECONDS, it probably didn't work.  */
static clock_t
sleep_int (clock_t clockticks)
{
  clock_t before, after, slept, remaining;
  sigset_t set, oset;
  struct sigaction act, oact;
  int save = errno;

  if (clockticks == 0)
    return 0;

  /* alarm() does not work in a TaskWindow nor whilst running as a
     WIMP program.  Note that when __taskwindow == 1 => __taskhandle != 0
     but not necessary vice-versa so the test on __taskhandle is enough. */
  if (__taskhandle != 0)
    {
      before = clock () + clockticks;
      while (clock () < before)
	;
      return 0;
    }

  /* Block SIGALRM signals while frobbing the handler.  */
  if (sigemptyset (&set) < 0
      || sigaddset (&set, SIGALRM) < 0
      || sigprocmask (SIG_BLOCK, &set, &oset))
    return clockticks;

  act.sa_handler = sleep_handler;
  act.sa_flags = 0;
  act.sa_mask = oset; /* Execute handler with original mask.  */
  if (sigaction (SIGALRM, &act, &oact))
    return clockticks;

  before = clock ();
  remaining = (ualarm ((useconds_t) clockticks * USECS_PER_CLOCK, 0)
	       / USECS_PER_CLOCK);

#ifdef DEBUG
  __os_print ("sleep: Set up an alarm for "); __os_prdec (clockticks);
  __os_print (" clockticks\r\n  Remaining: "); __os_prdec(remaining);
  __os_print (" clockticks\r\n");
#endif

  if (remaining > 0 && remaining < clockticks)
    {
      /* The user's alarm will expire before our own would.
         Restore the user's signal action state and let his alarm happen.  */
      sigaction (SIGALRM, &oact, (struct sigaction *) NULL);

      /* Restore sooner alarm.  */
      ualarm ((useconds_t) clockticks * USECS_PER_CLOCK, 0);
#ifdef DEBUG
      __os_print ("sleep: A user alarm existed. Wait ");
      __os_prdec (remaining); __os_print (" clockticks for that instead\r\n");
#endif
      sigsuspend (&oset);	/* Wait for it to go off.  */
#ifdef DEBUG
      __os_print ("sleep: Alarm has gone off. Continuing with execution\r\n");
#endif
      after = clock ();
    }
  else
    {
      /* Atomically restore the old signal mask
         (which had better not block SIGALRM),
         and wait for a signal to arrive.  */
#ifdef DEBUG
      __os_print ("sleep: Waiting for the alarm\r\n");
#endif
      sigsuspend (&oset);
#ifdef DEBUG
      __os_print ("sleep: Alarm has gone off. Continuing with execution\r\n");
#endif
      after = clock ();

      /* Restore the old signal action state.  */
      sigaction (SIGALRM, &oact, (struct sigaction *) NULL);
    }

  /* Notice how long we actually slept.  */
  slept = after - before;

  /* Restore the user's alarm if we have not already past it.
     If we have, be sure to turn off the alarm in case a signal
     other than SIGALRM was what woke us up.  */
  (void) ualarm (remaining > slept ? (useconds_t)(remaining - slept) * USECS_PER_CLOCK : 0, 0);

  /* Restore the original signal mask.  */
  (void) sigprocmask (SIG_SETMASK, &oset, (sigset_t *) NULL);

  /* Restore the `errno' value we started with.
     Some of the calls we made might have failed, but we don't care.  */
  (void) __set_errno (save);

  return slept > clockticks ? 0 : clockticks - slept;
}

/* Make the process sleep for SECONDS seconds, or until a signal arrives
   and is not ignored.  The function returns the number of seconds less
   than SECONDS which it actually slept (zero if it slept the full time).  */
unsigned int
sleep (unsigned int seconds)
{
  PTHREAD_SAFE_CANCELLATION

  return (unsigned int) sleep_int ((clock_t)seconds * CLOCKS_PER_SEC) / CLOCKS_PER_SEC;
}

int
usleep (useconds_t usec)
{
  PTHREAD_SAFE_CANCELLATION

  /* An allowed & specified limitation. Otherwise our calculations might
     overflow.  */
  if (usec >= 1000000)
    __set_errno (EINVAL);

  return (int) sleep_int ((usec + USECS_PER_CLOCK-1) / USECS_PER_CLOCK) * USECS_PER_CLOCK;
}
