/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/resource/c/setprior,v $
 * $Date: 1997/10/09 20:00:26 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: setprior,v 1.5 1997/10/09 20:00:26 unixlib Exp $";
#endif

#include <errno.h>
#include <sys/resource.h>
#include <sys/unix.h>
#include <limits.h>

/* A return value of zero indicates success.  */

int
setpriority (enum __priority_which which, int who, int prio)
{
  int i;

  if (who == 0)
    {
      /* Set the priority of the current process.  */
      switch (which)
	{
	case PRIO_PROCESS:
	  __u->ppri = (prio > PRIO_MAX) ? PRIO_MAX
	    : (prio < PRIO_MIN) ? PRIO_MIN : prio;
	  return 0;
	  break;
	case PRIO_PGRP:
	  __u->gpri = (prio > PRIO_MAX) ? PRIO_MAX
	    : (prio < PRIO_MIN) ? PRIO_MIN : prio;
	  return 0;
	  break;
	case PRIO_USER:
	  __u->upri = (prio > PRIO_MAX) ? PRIO_MAX
	    : (prio < PRIO_MIN) ? PRIO_MIN : prio;
	  return 0;
	  break;
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
	  if (__u->child[i].pid == who)
	    {
	      if (__u->child[i].gpri > prio)
		{
		  errno = EACCES;
		  return -1;
		}
	      __u->child[i].ppri = (prio > PRIO_MAX) ? PRIO_MAX
		: (prio < PRIO_MIN) ? PRIO_MIN : prio;
	      return 0;
	    }
	  break;
	case PRIO_PGRP:
	  if (__u->child[i].gid == who)
	    {
	      if (__u->child[i].gpri > prio)
		{
		  errno = EACCES;
		  return -1;
		}
	      __u->child[i].gpri = (prio > PRIO_MAX) ? PRIO_MAX
		: (prio < PRIO_MIN) ? PRIO_MIN : prio;
	      return 0;
	    }
	  break;
	case PRIO_USER:
	  if (__u->child[i].uid == who)
	    {
	      if (__u->child[i].upri > prio)
		{
		  errno = EACCES;
		  return -1;
		}
	      __u->child[i].upri = (prio > PRIO_MAX) ? PRIO_MAX
		: (prio < PRIO_MIN) ? PRIO_MIN : prio;
	      return 0;
	    }
	  break;
	default:
	  errno = EINVAL;
	  return -1;
	}
    }
  /* Value of 'which' was invalid.  */
  errno = ESRCH;
  return -1;
}
