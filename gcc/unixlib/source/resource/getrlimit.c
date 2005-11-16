/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/getrlimit.c,v $
 * $Date: 2002/02/14 15:56:36 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <sys/resource.h>
#include <unixlib/os.h>
#include <unixlib/unix.h>
#include <errno.h>
#include <stdio.h>

/* Put the soft and hard limits for RESOURCE in *RLIMITS.
   Returns 0 if successful, -1 if not (and sets errno).  */
int
getrlimit (enum __rlimit_resource resource, struct rlimit *rlimits)
{
  if (rlimits == NULL || (unsigned int) resource >= RLIMIT_NLIMITS)
    {
      errno = EINVAL;
      return -1;
    }

  *rlimits = __u->limit[resource];
  return 0;
}
