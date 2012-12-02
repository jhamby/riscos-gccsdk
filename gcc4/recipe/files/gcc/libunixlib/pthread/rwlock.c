/* Read/write lock routines. Most of the functionality is implemented in lock.c.
   Written by Alex Waugh.
   Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers.  */

#include <errno.h>
#include <pthread.h>

/* Initialise a rwlock object with the specified attributes */
int
pthread_rwlock_init (pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr)
{
  if (rwlock == NULL)
    return EINVAL;

  if (rwlock->type == LOCK_READ || rwlock->type == LOCK_WRITE)
    return EBUSY;

  rwlock->count = 0;
  rwlock->owner = NULL;
  rwlock->type = LOCK_READ;
  rwlock->waiting = NULL;

  if (attr != NULL)
    {
      /* Use supplied attributes */
      rwlock->attr = *attr;
    }
  else
    {
      /* Use default attributes */
      rwlock->attr.pshared = PTHREAD_PROCESS_PRIVATE;
      rwlock->attr.type = PTHREAD_MUTEX_DEFAULT;
    }

  return 0;
}

/* Destroy a rwlock that is finished with */
int
pthread_rwlock_destroy (pthread_rwlock_t *rwlock)
{
  if (rwlock == NULL)
    return EINVAL;

  if (rwlock->type != LOCK_READ && rwlock->type != LOCK_WRITE)
    return EINVAL;

  if (rwlock->count != 0 || rwlock->waiting != NULL)
    return EBUSY;

  rwlock->type = LOCK_INVALID;

  return 0;
}

/* Try to lock a rwlock for reading, blocking if unavailable */
int
pthread_rwlock_rdlock (pthread_rwlock_t *rwlock)
{
  if (rwlock == NULL || (rwlock->type != LOCK_WRITE && rwlock->type != LOCK_READ))
    return EINVAL;

  return __pthread_lock_lock (rwlock, LOCK_READ, 0);
}

/* Try to lock a rwlock for writing, blocking if unavailable */
int
pthread_rwlock_wrlock (pthread_rwlock_t *rwlock)
{
  if (rwlock == NULL || (rwlock->type != LOCK_WRITE && rwlock->type != LOCK_READ))
    return EINVAL;

  return __pthread_lock_lock (rwlock, LOCK_WRITE, 0);
}

/* Try to lock a rwlock for reading, but don't block if it is unavailable */
int
pthread_rwlock_tryrdlock (pthread_rwlock_t *rwlock)
{
  if (rwlock == NULL || (rwlock->type != LOCK_WRITE && rwlock->type != LOCK_READ))
    return EINVAL;

  return __pthread_lock_lock (rwlock, LOCK_READ, 1);
}

/* Try to lock a rwlock for writing, but don't block if it is unavailable */
int
pthread_rwlock_trywrlock (pthread_rwlock_t *rwlock)
{
  if (rwlock == NULL || (rwlock->type != LOCK_WRITE && rwlock->type != LOCK_READ))
    return EINVAL;

  return __pthread_lock_lock (rwlock, LOCK_WRITE, 1);
}

/* Unlock a previously locked rwlock */
int
pthread_rwlock_unlock (pthread_rwlock_t *rwlock)
{
  return __pthread_lock_unlock (rwlock,1);
}
