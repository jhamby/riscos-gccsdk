/* Mutex routines. Most of the functionality is implemented in lock.c.
   Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers
   Written by Alex Waugh */


#include <errno.h>
#include <pthread.h>

/* Initialise a mutex object with the specified attributes */
int
pthread_mutex_init (pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
  if (mutex == NULL)
    return EINVAL;

  mutex->count = 0;
  mutex->owner = NULL;
  mutex->type = LOCK_MUTEX;
  mutex->waiting = NULL;

  if (attr != NULL)
    {
      /* Use supplied attributes */
      mutex->attr = *attr;
    }
  else
    {
      /* Use default attributes */
      mutex->attr.pshared = PTHREAD_PROCESS_PRIVATE;
      mutex->attr.type = PTHREAD_MUTEX_DEFAULT;
    }

  return 0;
}

/* Destroy a mutex that is finished with */
int
pthread_mutex_destroy (pthread_mutex_t *mutex)
{
  if (mutex == NULL)
    return EINVAL;

  if (mutex->type != LOCK_MUTEX)
    return EINVAL;

  if (mutex->count != 0 || mutex->waiting != NULL)
    return EBUSY;

  mutex->type = LOCK_INVALID;

  return 0;
}

/* Try to lock a mutex, but don't block if it is unavailable */
int
pthread_mutex_trylock (pthread_mutex_t *mutex)
{
  if (mutex == NULL || mutex->type != LOCK_MUTEX)
    return EINVAL;

  return __pthread_lock_lock (mutex, LOCK_MUTEX, 1);
}

/* Lock a mutex, blocking if it is not available */
int
pthread_mutex_lock (pthread_mutex_t *mutex)
{
  if (mutex == NULL || mutex->type != LOCK_MUTEX)
    return EINVAL;

  return __pthread_lock_lock (mutex, LOCK_MUTEX, 0);
}

/* Unlock a previously locked mutex */
int
pthread_mutex_unlock (pthread_mutex_t *mutex)
{
  return __pthread_lock_unlock (mutex,1);
}
