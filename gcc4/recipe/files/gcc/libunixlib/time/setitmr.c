/* setitimer ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <stddef.h>
#include <errno.h>
#include <swis.h>
#include <sys/time.h>

#include <internal/os.h>
#include <internal/unix.h>
#include <pthread.h>

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

typedef void (*ticker) (void);

static __inline void
remove_ticker (ticker address, const struct timeval *old)
{
  int regs[10];

  regs[0] = (int) address;
  regs[1] = (int) old;
  __os_swi (OS_RemoveTickerEvent, regs);
}

static __inline int
add_ticker (const struct timeval *tv, ticker address,
	    const struct timeval *newtime)
{
  int regs[10];

  regs[0] = (int) (tv->tv_sec * 100) + (tv->tv_usec + 9999) / 10000;
  regs[1] = (int) address;
  regs[2] = (int) newtime;
  return __os_swi (OS_CallAfter, regs) ? -1 : 0;
}

static int
check_ticker (const struct timeval *tv)
{
  return (tv->tv_sec > 0 || tv->tv_usec > 0) ? 1
	  : (tv->tv_sec == 0 && tv->tv_usec == 0) ? 2
	  : 0;
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

int
setitimer (enum __itimer_which which, const struct itimerval *new_timer,
	   struct itimerval *old_timer)
{
  struct itimerval *itimer;

  PTHREAD_UNSAFE

  /* We can't implement interval timers whilst executing in a task window
     nor whilst running as a WIMP program.  */
  if (__get_taskhandle () != 0)
    return __set_errno (ENOSYS);

  if ((unsigned) which >= __MAX_ITIMERS
      || new_timer->it_interval.tv_usec >= 1000000
      || new_timer->it_value.tv_usec >= 1000000)
    return __set_errno (EINVAL);

  /* __u is current process <internal/unix.h>.  */
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
}
