/* Thread attribute manipulation functions.
   Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers
   Written by Alex Waugh */

#include <pthread.h>
#include <errno.h>

/* Initialise a thread attributes object with default values */
int
pthread_attr_init (pthread_attr_t *attr)
{
  if (attr == NULL)
    return EINVAL;

  attr->detachstate = PTHREAD_CREATE_JOINABLE;
  attr->guardsize = 0;
  attr->stackaddr = NULL;
  attr->stacksize = PTHREAD_STACK_MIN;

  return 0;
}

/* Destroy a thread attributes object (does nothing) */
int
pthread_attr_destroy (pthread_attr_t *attr)
{
  attr = attr;

  return 0;
}


/* Sets the detach state attribute */
int
pthread_attr_setdetachstate (pthread_attr_t *attr, int detachstate)
{
  if (attr == NULL)
    return EINVAL;

  if (detachstate != PTHREAD_CREATE_DETACHED && detachstate != PTHREAD_CREATE_JOINABLE)
    return EINVAL;

  attr->detachstate = detachstate;

  return 0;
}

/* Gets the detach state attribute */
int
pthread_attr_getdetachstate (const pthread_attr_t *attr, int *detachstate)
{
  if (attr == NULL || detachstate == NULL)
    return EINVAL;

  *detachstate = attr->detachstate;

  return 0;
}

/* Sets the size of the stack guard attribute
   The stack guard is an area of memory at the end of the stack to
   provide protection against stack overflow. This is ignored as
   the stack is extensible. */
int
pthread_attr_setguardsize (pthread_attr_t *attr, size_t guardsize)
{
  if (attr == NULL)
    return EINVAL;

  attr->guardsize = guardsize;

  return 0;
}

/* Gets the size of the stack guard attribute */
int
pthread_attr_getguardsize (const pthread_attr_t *attr, size_t *guardsize)
{
  if (attr == NULL || guardsize == NULL)
    return EINVAL;

  *guardsize = attr->guardsize;

  return 0;
}

/* Sets the address of a thread's stack
   This is ignored as there are limitations on what can be used
   for the stack. */
int
pthread_attr_setstackaddr (pthread_attr_t *attr, void *stackaddr)
{
  if (attr == NULL)
    return EINVAL;

  attr->stackaddr = stackaddr;

  return 0;
}

/* Gets the address of a thread's stack */
int
pthread_attr_getstackaddr (const pthread_attr_t *attr, void **stackaddr)
{
  if (attr == NULL || stackaddr == NULL)
    return EINVAL;

  *stackaddr = attr->stackaddr;

  return 0;
}

/* Sets the size of a thread's stack (ignored) */
int
pthread_attr_setstacksize (pthread_attr_t *attr, size_t stacksize)
{
  if (attr == NULL)
    return EINVAL;

  if (stacksize < PTHREAD_STACK_MIN)
    return EINVAL;

  attr->stacksize = stacksize;

  return 0;
}

/* Gets the size of a thread's stack (as set with setstacksize) */
int
pthread_attr_getstacksize (const pthread_attr_t *attr, size_t *stacksize)
{
  if (attr == NULL || stacksize == NULL)
    return EINVAL;

  *stacksize = attr->stacksize;

  return 0;
}

/* Set scheduling parameter attributes (currently ignored) */
int
pthread_attr_setschedparam (pthread_attr_t *attr, const struct sched_param *param)
{
  if (attr == NULL || param == NULL)
    return EINVAL;

  attr->param = *param;

  return 0;
}

/* Get scheduling parameter attributes */
int
pthread_attr_getschedparam (const pthread_attr_t *attr, struct sched_param *param)
{
  if (attr == NULL || param == NULL)
    return EINVAL;

  *param = attr->param;

  return 0;
}
