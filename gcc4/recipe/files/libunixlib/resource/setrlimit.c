/* setrlimit ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <sys/resource.h>
#include <errno.h>

#include <pthread.h>
#include <internal/unix.h>

/* Set the soft and hard limits for RESOURCE to *RLIMITS.
   Only the super-user can increase hard limits.
   Return 0 if successful, -1 if not (and sets errno).  */
int
setrlimit (enum __rlimit_resource resource, const struct rlimit *rlimits)
{
  struct rlimit lim;

  PTHREAD_UNSAFE

  if (rlimits == NULL || (unsigned int) resource >= RLIMIT_NLIMITS)
    return __set_errno (EINVAL);

  lim = *rlimits;

#if 0
  if (lim.rlim_max != RLIM_INFINITY)
    {
      /* We have no enforceable resource limits.  */
      return __set_errno (ENOSYS);
    }
#endif
  if (lim.rlim_cur > lim.rlim_max)
    lim.rlim_cur = lim.rlim_max;

  __u->limit[resource] = lim;

  return 0;
}
