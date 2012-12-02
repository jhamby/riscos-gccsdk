/* Mutex attribute manipulation routines.
   Written by Alex Waugh.
   Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers.  */

#include <errno.h>
#include <pthread.h>

/* Initialise an attributes object */
int
pthread_mutexattr_init (pthread_mutexattr_t *attr)
{
  if (attr == NULL)
    return EINVAL;

  attr->pshared = PTHREAD_PROCESS_PRIVATE;
  attr->type = PTHREAD_MUTEX_DEFAULT;

  return 0;
}

/* Destroy an attributes object */
int
pthread_mutexattr_destroy (pthread_mutexattr_t *attr)
{
  if (attr == NULL)
    return EINVAL;

  return 0;
}

/* Get the process shared attribute */
int
pthread_mutexattr_getpshared (const pthread_mutexattr_t *attr, int *pshared)
{
  if (attr == NULL || pshared == NULL)
    return EINVAL;

  *pshared = attr->pshared;

  return 0;
}

/* Set the process shared attribute (Ignored under RISC OS) */
int
pthread_mutexattr_setpshared (pthread_mutexattr_t *attr, int pshared)
{
  if (attr == NULL)
    return EINVAL;

  if (pshared != PTHREAD_PROCESS_PRIVATE && pshared != PTHREAD_PROCESS_SHARED)
    return EINVAL;

  attr->pshared = pshared;

  return 0;
}

/* Get the type of mutex */
int
pthread_mutexattr_gettype (const pthread_mutexattr_t *attr, int *type)
{
  if (attr == NULL || type == NULL)
    return EINVAL;

  *type = attr->type;

  return 0;
}

/* Set the type of mutex (specifies the behaviour when a thread recursively locks the mutex) */
int
pthread_mutexattr_settype (pthread_mutexattr_t *attr, int type)
{
  if (attr == NULL)
    return EINVAL;

  switch (type)
    {
      /* case PTHREAD_MUTEX_DEFAULT: is one of the other three */
      case PTHREAD_MUTEX_NORMAL:
      case PTHREAD_MUTEX_ERRORCHECK:
      case PTHREAD_MUTEX_RECURSIVE:
        break;
      default:
        return EINVAL;
    }

  attr->type = type;

  return 0;
}
