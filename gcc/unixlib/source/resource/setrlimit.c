/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/setrlimit.c,v $
 * $Date: 2000/07/15 14:52:28 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: setrlimit.c,v 1.1.1.1 2000/07/15 14:52:28 nick Exp $";
#endif

#include <sys/resource.h>
#include <sys/unix.h>
#include <errno.h>


/* Set the soft and hard limits for RESOURCE to *RLIMITS.
   Only the super-user can increase hard limits.
   Return 0 if successful, -1 if not (and sets errno).  */
int
setrlimit (enum __rlimit_resource resource, const struct rlimit *rlimits)
{
  struct rlimit lim;

  if (rlimits == NULL || (unsigned int) resource >= RLIMIT_NLIMITS)
    {
      errno = EINVAL;
      return -1;
    }

  lim = *rlimits;

#if 0
  if (lim.rlim_max != RLIM_INFINITY)
    {
      /* We have no enforceable resource limits.  */
      errno = ENOSYS;
      return -1;
    }
#endif
  if (lim.rlim_cur > lim.rlim_max)
    lim.rlim_cur = lim.rlim_max;

  __u->limit[resource] = lim;

  return 0;
}
