/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sleep.c,v $
 * $Date: 2000/07/15 14:52:30 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sleep.c,v 1.1.1.1 2000/07/15 14:52:30 nick Exp $";
#endif

/* signal.c.sleep: Written by Nick Burrett, 6 October 1996.  */

#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/syslib.h>

/* SIGALRM signal handler for `sleep'.  This does nothing but return,
   but SIG_IGN isn't supposed to break `pause'.  */
static void
sleep_handler (int sig)
{
  sig = sig;
  return;
}

/* Make the process sleep for SECONDS seconds, or until a signal arrives
   and is not ignored.  The function returns the number of seconds less
   than SECONDS which it actually slept (zero if it slept the full time).  */
unsigned int
sleep (unsigned int seconds)
{
  unsigned int remaining, slept;
  time_t before, after;
  sigset_t set, oset;
  struct sigaction act, oact;
  int save = errno;

  if (seconds == 0)
    return 0;

  /* alarm does not work in a TaskWindow.  */
  if (__taskwindow)
    {
      before = time ((time_t *) NULL) + seconds;
      while (time ((time_t *) NULL) < before)
        ;
      return 0;
    }

  /* Block SIGALRM signals while frobbing the handler.  */
  sigemptyset (&set);
  if (sigaddset (&set, SIGALRM) || sigprocmask (SIG_BLOCK, &set, &oset))
    return seconds;

  act.sa_handler = sleep_handler;
  act.sa_flags = 0;
  sigemptyset (&act.sa_mask);
  if (sigaction (SIGALRM, &act, &oact))
    return seconds;

  before = time ((time_t *) NULL);
  remaining = alarm (seconds);

#ifdef DEBUG
  os_print ("sleep: Set up an alarm for "); os_prdec (seconds);
  os_print (" seconds\r\n");
#endif

  if (remaining > 0 && remaining < seconds)
    {
      /* The user's alarm will expire before our own would.
         Restore the user's signal action state and let his alarm happen.  */
      sigaction (SIGALRM, &oact, (struct sigaction *) NULL);
      alarm (remaining);	/* Restore sooner alarm.  */
#ifdef DEBUG
      os_print ("sleep: A user alarm existed. Wait ");
      os_prdec (remaining); os_print (" secs for that instead\r\n");
#endif
      sigsuspend (&oset);	/* Wait for it to go off.  */
#ifdef DEBUG
      os_print ("sleep: Alarm has gone off. Continuing with execution\r\n");
#endif
      after = time ((time_t *) NULL);
    }
  else
    {
      /* Atomically restore the old signal mask
         (which had better not block SIGALRM),
         and wait for a signal to arrive.  */
#ifdef DEBUG
      os_print ("sleep: Waiting for the alarm\r\n");
#endif
      sigsuspend (&oset);
#ifdef DEBUG
      os_print ("sleep: Alarm has gone off. Continuing with execution\r\n");
#endif
      after = time ((time_t *) NULL);

      /* Restore the old signal action state.  */
      sigaction (SIGALRM, &oact, (struct sigaction *) NULL);
    }

  /* Notice how long we actually slept.  */
  slept = (unsigned int) (after - before);

  /* Restore the user's alarm if we have not already past it.
     If we have, be sure to turn off the alarm in case a signal
     other than SIGALRM was what woke us up.  */
  alarm (remaining > slept ? remaining - slept : 0);

  /* Restore the original signal mask.  */
  sigprocmask (SIG_SETMASK, &oset, (sigset_t *) NULL);

  /* Restore the `errno' value we started with.
     Some of the calls we made might have failed, but we don't care.  */
  (void) __set_errno (save);

  return slept > seconds ? 0 : seconds - slept;
}
