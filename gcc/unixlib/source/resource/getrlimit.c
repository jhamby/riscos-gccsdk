/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/resource/c/getrlimit,v $
 * $Date: 1997/10/09 20:00:26 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getrlimit,v 1.5 1997/10/09 20:00:26 unixlib Exp $";
#endif

#include <sys/syslib.h>
#include <sys/resource.h>
#include <sys/os.h>
#include <sys/unix.h>
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
