/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/getrlimit.c,v $
 * $Date: 2000/07/15 14:52:28 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getrlimit.c,v 1.1.1.1 2000/07/15 14:52:28 nick Exp $";
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
