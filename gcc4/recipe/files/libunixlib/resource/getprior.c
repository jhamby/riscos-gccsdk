/* getpriority ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <sys/resource.h>
#include <limits.h>

#include <internal/unix.h>

/* Return the highest priority of any process specified by 'which' and 'who'
   (see <sys/resource.h>); if WHO is zero, the current process, process group,
   or user (as specified by WHO) is used.  A lower priority number means higher
   priority.  Priorities range from PRIO_MIN to PRIO_MAX.  */
int
getpriority (enum __priority_which which, id_t who)
{
  const struct __sul_process *sulproc = __ul_global.sulproc;
  const struct __sul_process *child;
  int ret;

  if (who == 0)
    {
      /* Return priorities of the current process.  */
      switch (which)
	{
	case PRIO_PROCESS:
	  return sulproc->ppri;
	case PRIO_PGRP:
	  return sulproc->gpri;
	case PRIO_USER:
	  return sulproc->upri;
	}

      return __set_errno (EINVAL);
    }

  for (child = sulproc->children, ret = -1;
       child != NULL && ret == -1;
       child = child->next_child)
    {
      switch (which)
	{
	case PRIO_PROCESS:
	  if (child->pid == who)
	    ret = child->ppri;
	  break;
	case PRIO_PGRP:
	  if (child->pgrp == who)
	    ret = child->gpri;
	  break;
	case PRIO_USER:
	  if (child->euid == who)
	    ret = child->upri;
	  break;
	default:
	  return __set_errno (EINVAL);
	}
    }

  if (ret != -1)
    return ret;

  /* Invalid value of 'which'.  */
  return __set_errno (ESRCH);
}
