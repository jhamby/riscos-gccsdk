/* Control thread scheduling parameters.
   Copyright (c) 2005 UnixLib Developers.  */

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

  thread->param = *param;
  return 0;
}

/* Get scheduling parameter. */
int pthread_getschedparam (const pthread_t thread,
			   int policy, struct sched_param *param)
{
  if (thread == NULL || param == NULL)
    return EINVAL;

  if (policy != SCHED_FIFO
      || policy != SCHED_RR
      || policy != SCHED_SPORADIC
      || policy != SCHED_OTHER)
    return EINVAL;

  *param = thread->param;
  return 0;
}
