/* Control thread scheduling parameters.
   Copyright (c) 2005, 2006 UnixLib Developers.  */

#include <errno.h>
#include <pthread.h>
#include <sched.h>

/* Set scheduling parameter (currently ignored) */
int pthread_setschedparam (pthread_t thread, int policy,
			   const struct sched_param *param)
{
  if (thread == NULL || param == NULL)
    return EINVAL;

  if (policy != SCHED_FIFO
      || policy != SCHED_RR
      || policy != SCHED_SPORADIC
      || policy != SCHED_OTHER)
    return EINVAL;

  thread->__policy = policy;
  thread->__param = *param;
  return 0;
}

/* Get scheduling parameter. */
int pthread_getschedparam (pthread_t thread,
			   int *policy,
			   struct sched_param *param)
{
  if (thread == NULL || policy == NULL || param == NULL)
    return EINVAL;

  *policy = thread->__policy;
  *param = thread->__param;
  return 0;
}
