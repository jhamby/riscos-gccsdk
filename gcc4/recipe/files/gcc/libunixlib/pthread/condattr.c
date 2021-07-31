/* Condition variable attributes manipulation.
   Written by Alex Waugh.
   Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers.  */

#include <errno.h>
#include <pthread.h>

/* Initialise a condition variable attributes object */
int
pthread_condattr_init (pthread_condattr_t *attr)
{
  if (attr == NULL)
    return EINVAL;

  attr->pshared = PTHREAD_PROCESS_PRIVATE;
  attr->clock_id = CLOCK_REALTIME;

  return 0;
}

/* Destroy an attributes object */
int
pthread_condattr_destroy (pthread_condattr_t *attr)
{
  if (attr == NULL)
    return EINVAL;

  return 0;
}

/* Get process shared attribute */
int
pthread_condattr_getpshared (const pthread_condattr_t *attr, int *pshared)
{
  if (attr == NULL || pshared == NULL)
    return EINVAL;

  *pshared = attr->pshared;

  return 0;
}

/* Set process shared attribute (ignored under RISC OS) */
int
pthread_condattr_setpshared (pthread_condattr_t *attr, int pshared)
{
  if (attr == NULL)
    return EINVAL;

  if (pshared != PTHREAD_PROCESS_PRIVATE && pshared != PTHREAD_PROCESS_SHARED)
    return EINVAL;

  attr->pshared = pshared;

  return 0;
}

int pthread_condattr_getclock(const pthread_condattr_t *restrict attr,
			      clockid_t *restrict clock_id)
{
  if (attr == NULL || clock_id == NULL)
    return EINVAL;

  *clock_id = attr->clock_id;

  return 0;
}

int pthread_condattr_setclock(pthread_condattr_t *attr, clockid_t clock_id)
{
  if (attr == NULL)
    return EINVAL;

  if (clock_id != CLOCK_REALTIME && clock_id != CLOCK_MONOTONIC)
    return EINVAL;

  attr->clock_id = clock_id;

  return 0;
}
