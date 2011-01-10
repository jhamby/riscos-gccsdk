/* getrlimit ()
 * Copyright (c) 2000-2011 UnixLib Developers
 */

#include <sys/resource.h>
#include <errno.h>
#include <stdio.h>

#include <internal/os.h>
#include <internal/unix.h>

/* Put the soft and hard limits for RESOURCE in *RLIMITS.
   Returns 0 if successful, -1 if not (and sets errno).  */
int
getrlimit (__rlimit_resource_t resource, struct rlimit *rlimits)
{
  if (rlimits == NULL || (unsigned int) resource >= RLIMIT_NLIMITS)
    return __set_errno (EINVAL);

  *rlimits = __u->limit[resource];
  return 0;
}
