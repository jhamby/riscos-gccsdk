/* Condition variables
   Written by Alex Waugh.
   Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers.  */

#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include <internal/os.h>
#include <pthread.h>

#ifdef PTHREAD_DEBUG
#  include <sys/debug.h>
#endif

/* Initialise a condition variable with the given attributes */
int
pthread_cond_init (pthread_cond_t *cond, const pthread_condattr_t *attr)
{
  attr = attr;

  if (cond == NULL)
    return EINVAL;

  cond->waiting = NULL;

  return 0;
}

/* Destroy a condition variable */
int
pthread_cond_destroy (pthread_cond_t *cond)
{
  if (cond == NULL)
    return EINVAL;

  return 0;
}

/* Wait for a condition variable to be signalled */
int
pthread_cond_wait (pthread_cond_t *cond, pthread_mutex_t *mutex)
{
  return pthread_cond_timedwait (cond, mutex, NULL);
}

/* Wait for a condition variable to be signalled, with a timeout */
int
pthread_cond_timedwait (pthread_cond_t *cond, pthread_mutex_t *mutex,
                        const struct timespec *abstime)
{
  pthread_t thread;

  if (cond == NULL || mutex == NULL)
    return EINVAL;

  if (mutex->owner != __pthread_running_thread)
    return EINVAL;

  __pthread_disable_ints ();

  /* This call will not return an error unless mutex is NULL, which we've
     already checked for */
  __pthread_lock_unlock (mutex,0);

  __pthread_running_thread->cond = cond;
  __pthread_running_thread->nextwait = NULL;

  if (abstime == NULL)
    {
      /* No timeout */
      __pthread_running_thread->state = STATE_COND_WAIT;
      __pthread_running_thread->condtimeout = 0;
    }
  else
    {
      clock_t diff;

      __pthread_running_thread->state = STATE_COND_TIMED_WAIT;

      /* A bit crude, but... */
      diff = (100 * (abstime->tv_sec - time (NULL)))
             + (abstime->tv_nsec / 10000000);
      __pthread_running_thread->condtimeout = clock () + diff;
    }

  /* Add this thread to the linked list of threads that are waiting on
     this condition var */
  thread = cond->waiting;
  if (thread == NULL)
    {
      cond->waiting = __pthread_running_thread;
    }
  else
    {
      while (thread->nextwait != NULL)
        thread = thread->nextwait;

      thread->nextwait = __pthread_running_thread;
    }

  __pthread_enable_ints ();

  pthread_yield ();
  /* pthread_yield won't return until either the condition var has been signalled, or a timeout occured */

  if (abstime != NULL)
    {
      if (clock () > __pthread_running_thread->condtimeout)
        {
          pthread_mutex_lock (mutex);
          return ETIMEDOUT;
        }
    }

  return pthread_mutex_lock (mutex);
}

/* Signal the specified condition var */
int
pthread_cond_signal (pthread_cond_t *cond)
{
  pthread_t thread;

  if (cond == NULL)
    return EINVAL;

  __pthread_disable_ints ();

  /* Set the first thread waiting on the condition var to a runnable state */
  thread = cond->waiting;
  if (thread != NULL)
    {
#ifdef PTHREAD_DEBUG
      debug_printf ("-- pthread_cond_signal: Signalling thread %p, next waiting = %p\n", thread, thread->nextwait);
#endif
      thread->state = STATE_RUNNING;
      cond->waiting = thread->nextwait;
    }
#ifdef PTHREAD_DEBUG
  else
    debug_printf ("No thread to signal\n");
#endif

  __pthread_enable_ints ();
  return 0;
}

/* Broadcast a signal to all threads waiting on this conition var */
int pthread_cond_broadcast(pthread_cond_t *cond)
{
  pthread_t thread;

  if (cond == NULL)
    return EINVAL;

  __pthread_disable_ints ();

  thread = cond->waiting;

  /* Set all threads waiting on the condition var to a runnable state */
  while (thread != NULL)
    {
      thread->state = STATE_RUNNING;
      thread = thread->nextwait;
    }

  cond->waiting = NULL;

  __pthread_enable_ints ();
  return 0;
}

