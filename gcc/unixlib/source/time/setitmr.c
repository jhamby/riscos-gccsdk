/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/setitmr.c,v $
 * $Date: 2000/07/15 14:52:42 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: setitmr.c,v 1.1.1.1 2000/07/15 14:52:42 nick Exp $";
#endif

#include <stddef.h>
#include <errno.h>
#include <sys/os.h>
#include <sys/swis.h>
#include <sys/time.h>
#include <sys/unix.h>
#include <sys/syslib.h>

/* setitimer provides a mechanism for a process to interrupt itself at
   some future time. This is achieved by setting a timer; when the
   timer expires, the process receives a signal.

   Each process has three independent interval timers available:

   ITIMER_REAL: A real-time timer that counts clock time. This timer
   sends a SIGALRM signal to the process when it expires.

   ITIMER_VIRTUAL: A virtual timer that counts CPU time used by the
   process. This timer sends a SIGVTALRM signal to the process when it
   expires.

   ITIMER_PROF: A profiling timer that counts both CPU time used by
   the process, and CPU time spent in systems calls on behalf of the
   process. This timer sends a SIGPROF signal to the process when it
   expires.

   Only one timer of each kind can be set at any given time. If a timer
   is set that has not yet expired, that timer is reset to the new value.

   The setitimer function sets the timer specified by WHICH
   according to NEW.  The WHICH argument can have a value of
   ITIMER_REAL, ITIMER_VIRTUAL, or ITIMER_PROF.

   If OLD is not a null pointer, setitimer returns information
   about any previous unexpired timer of the same kind in the
   structure it points to.

   The return value is 0 on success and -1 on failure.  */

#ifdef __FEATURE_ITIMERS
typedef void (*ticker) (void);

static void
remove_ticker (ticker address, const struct timeval *old)
{
  int regs[10];

  regs[0] = (int) address;
  regs[1] = (int) old;
  os_swi (OS_RemoveTickerEvent, regs);
}

static int
add_ticker (const struct timeval *time, ticker address,
	    const struct timeval *new)
{
  int regs[10];

  regs[0] = (int) (time->tv_sec * 100) + (time->tv_usec + 9999) / 10000;
  regs[1] = (int) address;
  regs[2] = (int) new;
  return os_swi (OS_CallAfter, regs) ? -1 : 0;
}

static int
check_ticker (const struct timeval *time)
{
  if (time->tv_sec > 0 || time->tv_usec > 0)
    return 1;
  if (time->tv_sec == 0 && time->tv_usec == 0)
    return 2;
  return 0;
}

struct timer_control
{
  ticker callback;
  int *sema;
};

static const struct timer_control timer_controls[__MAX_ITIMERS] =
{
  {__h_sigalrm_init, &__h_sigalrm_sema},	/* ITIMER_REAL */
  {__h_sigvtalrm_init, &__h_sigvtalrm_sema},	/* ITIMER_VIRTUAL */
  {__h_sigprof_init, &__h_sigprof_sema}		/* ITIMER_PROF */
};
#endif

int
setitimer (enum __itimer_which which, const struct itimerval *new_timer,
	   struct itimerval *old_timer)
{
#ifdef __FEATURE_ITIMERS
  struct itimerval *itimer;

  /* We can't implement interval timers whilst executing in a task window.  */
  if (__taskwindow)
    return __set_errno (ENOSYS);

  if ((unsigned) which >= __MAX_ITIMERS)
    return __set_errno (EINVAL);

  /* __u is current process <sys/unix.h>.  */
  itimer = &__u->itimers[which];
  if (old_timer)
    *old_timer = *itimer;

  /* Only if a timer is already active (it_value > 0), then remove it */
  if ((check_ticker (&itimer->it_value) == 1)
      || check_ticker (&itimer->it_interval))
    {
      remove_ticker (timer_controls[which].callback, &itimer->it_interval);
      *(timer_controls[which].sema) = 0;
    }

  *itimer = *new_timer;

  if (check_ticker (&new_timer->it_value) == 1)
    return add_ticker (&new_timer->it_value,
		       timer_controls[which].callback,
		       &itimer->it_interval);

  return 0;
#else
  return __set_errno (ENOSYS);
#endif
}
