/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/getprior.c,v $
 * $Date: 2001/09/04 16:32:04 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getprior.c,v 1.2.2.1 2001/09/04 16:32:04 admin Exp $";
#endif

#include <errno.h>
#include <sys/resource.h>
#include <unixlib/unix.h>
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
