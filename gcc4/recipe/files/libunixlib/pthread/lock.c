/* Common routines shared by the mutex, rwlock and cond functions.
   Copyright (c) 2002, 2003, 2004, 2005, 2006, 2008 UnixLib Developers
   Written by Alex Waugh */

/* #define PTHREAD_DEBUG */

#include <pthread.h>
#include <errno.h>
#include <limits.h>

#include <internal/os.h>

/* Mark a mutex/rwlock as owned by the current thread */
static void
__pthread_lock_acquire (pthread_mutex_t *mutex, const enum __pthread_locktype type)
{
  mutex->owner = __pthread_running_thread; /* There might be more than one owner for a read lock, but it doesn't matter */
  mutex->count++;
  mutex->type = type;
#ifdef PTHREAD_DEBUG
  debug_printf ("-- __pthread_lock_acquire: Incrementing %p to %u for %p\n", mutex, mutex->count, __pthread_running_thread);
#endif
}

/* Block till a mutex is available */
/* Ints should already be disabled on entry to this function */
static void
__pthread_lock_block (pthread_mutex_t *mutex, const enum __pthread_locktype type)
{
  pthread_t list = mutex->waiting;
  int wait;

  __pthread_running_thread->mutex = mutex;
  __pthread_running_thread->mutextype = type;
  __pthread_running_thread->nextwait = NULL;

  /* Add this thread to the end of the list of threads waiting on the mutex */
  if (list == NULL)
    mutex->waiting = __pthread_running_thread;
  else
    {
      while (list->nextwait != NULL)
        list = list->nextwait;

      list->nextwait = __pthread_running_thread;
    }

  do
    {
      __pthread_running_thread->state = STATE_MUTEX_WAIT;

#ifdef PTHREAD_DEBUG
      debug_printf ("-- __pthread_lock_block: Unavailable, yielding %p from %p\n", mutex, __pthread_running_thread);
#endif
      __pthread_enable_ints ();
      /* Yield until the mutex becomes available */
      pthread_yield ();
      __pthread_disable_ints ();

      if (mutex->owner == NULL)
        wait = 0; /* Lock is now available for us */
      else
        {
          if (type == LOCK_READ && mutex->type == LOCK_READ)
            wait = 0; /* Many read locks can be held at once, by different threads */
          else
            wait = 1; /* Lock is still not available */
        }
    }
  while (wait);

#ifdef PTHREAD_DEBUG
  debug_printf ("-- __pthread_lock_block: Acquired %p for %p\n",mutex,__pthread_running_thread);
#endif

  /* Remove the current thread from the list of threads waiting on the mutex */
  if (mutex->waiting == __pthread_running_thread)
    mutex->waiting = mutex->waiting->nextwait;
  else
    {
      for (list = mutex->waiting;
	   list != NULL && list->nextwait != __pthread_running_thread;
	   list = list->nextwait)
	/* */;

      if (list == NULL)
        __pthread_fatal_error ("-- __pthread_lock_block: Mutex does not have a thread waiting on it, but it should have");

      list->nextwait = __pthread_running_thread->nextwait;
    }

  __pthread_lock_acquire (mutex, type);

  __pthread_running_thread->mutex = NULL;
  __pthread_running_thread->nextwait = NULL;
}

/* Main routine for locking a mutex/rwlock */
int
__pthread_lock_lock (pthread_mutex_t *mutex, const enum __pthread_locktype type, const int trylock)
{
  __pthread_disable_ints ();

#ifdef PTHREAD_DEBUG
  debug_printf ("-- __pthread_lock_lock: %p type %d for %p\n", mutex, type, __pthread_running_thread);
#endif

  if (mutex->owner == NULL || (mutex->type == LOCK_READ && type == LOCK_READ))
    {
      /* Mutex is currently available */
      if (type == LOCK_READ)
        {
          /* Check there aren't any writers waiting on the lock */
          pthread_t list;
          int block = 0;

          for (list = mutex->waiting; list != NULL; list = list->nextwait)
            {
              if (list->mutextype == LOCK_WRITE)
                {
                  block = 1;
                  break;
                }
            }
          if (block)
            {
              /* There is at least one writer waiting for this lock, so block to prevent writer starvation */
              if (trylock)
                {
                  __pthread_enable_ints ();
                  return EBUSY;
                }
              __pthread_lock_block (mutex, type);
            }
          else
            {
              /* No writers waiting, so get the lock */
              __pthread_lock_acquire (mutex, type);
            }
        }
      else
        {
          /* We are a writer and the mutex is free, so get the lock */
          __pthread_lock_acquire (mutex, type);
        }
    }
  else if (mutex->owner == __pthread_running_thread && mutex->attr.type != PTHREAD_MUTEX_NORMAL)
    {
      /* Mutex is already held by the current thread */
      if (mutex->attr.type == PTHREAD_MUTEX_ERRORCHECK)
        {
          __pthread_enable_ints ();
          return trylock ? EBUSY : EDEADLK;
        }
      else
        {
          if (mutex->count == UINT_MAX)
            {
              __pthread_enable_ints ();
              return EAGAIN;
            }
          __pthread_lock_acquire (mutex, type);
        }
    }
  else
    {
      /* Mutex is held by a different thread */
      if (trylock)
        {
          __pthread_enable_ints ();
          return EBUSY;
        }

     /* Block till it becomes available */
      __pthread_lock_block (mutex, type);
    }

  __pthread_enable_ints ();
  return 0;
}

/* Main routine for unlocking a mutex/rwlock
   If yield is non zero, and there is another thread waiting
   on the mutex then yield once the lock has been given up,
   to prevent possible starvation */
int
__pthread_lock_unlock (pthread_mutex_t *mutex, int yield)
{
  pthread_t write = NULL;

  if (mutex == NULL)
    return EINVAL;

  __pthread_disable_ints ();

#ifdef PTHREAD_DEBUG
  debug_printf ("-- __pthread_lock_unlock: %p for %p\n", mutex, __pthread_running_thread);
#endif

  if ((mutex->type != LOCK_READ && mutex->owner != __pthread_running_thread) || mutex->count == 0)
    {
      __pthread_enable_ints ();
      return EPERM;
    }

  mutex->count--;
  if (mutex->count == 0)
    {
      pthread_t thread;

      mutex->owner = NULL;

      thread = mutex->waiting;

      /* Look to see if there are any writers blocked on this mutex */
      while (thread)
        {
          if (thread == __pthread_running_thread)
            __pthread_fatal_error ("-- __pthread_lock_unlock: Unlocking mutex that current thread is already blocked on");

          if (thread->mutex != mutex)
            __pthread_fatal_error ("-- __pthread_lock_unlock: Blocking thread in wrong mutex list");

          if (thread->mutextype == LOCK_WRITE || thread->mutextype == LOCK_MUTEX)
            {
              write = thread;
              break;
            }
          thread = thread->nextwait;
        }

      if (write != NULL)
        {
          /* A writer was found, so give it a chance to run */
          write->state = STATE_RUNNING;
        }
      else
        {
          /* No writers are waiting for this lock, so all must be readers, so let them all run */
          thread = mutex->waiting;

          while (thread)
            {
              thread->state = STATE_RUNNING;
              thread = thread->nextwait;
            }
        }
    }

  __pthread_enable_ints ();

  if (mutex->waiting != NULL && yield)
    pthread_yield ();

  return 0;
}
