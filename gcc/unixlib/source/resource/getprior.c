/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/resource/c/getprior,v $
 * $Date: 1997/10/09 20:00:26 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getprior,v 1.5 1997/10/09 20:00:26 unixlib Exp $";
#endif

#include <errno.h>
#include <sys/resource.h>
#include <sys/unix.h>
#include <limits.h>

/* Return the highest priority of any process specified by 'which' and 'who'
   (see <sys/resource.h>); if WHO is zero, the current process, process group,
   or user (as specified by WHO) is used.  A lower priority number means higher
   priority.  Priorities range from PRIO_MIN to PRIO_MAX.  */
int
getpriority (enum __priority_which which, int who)
{
  int i;

  if (who == 0)
    {
      /* Return priorities of the current process.  */
      switch (which)
	{
	case PRIO_PROCESS:
	  return __u->ppri;
	case PRIO_PGRP:
	  return __u->gpri;
	case PRIO_USER:
	  return __u->upri;
	default:
	  errno = EINVAL;
	  return -1;
	}
      return -1;
    }
  for (i = 0; i < CHILD_MAX; i++)
    {
      switch (which)
	{
	case PRIO_PROCESS:
	  return __u->child[i].ppri;
	case PRIO_PGRP:
	  return __u->child[i].gpri;
	case PRIO_USER:
	  return __u->child[i].upri;
	default:
	  errno = EINVAL;
	  return -1;
	}
    }
  /* Invalid value of 'which'.  */
  errno = ESRCH;
  return -1;
}
