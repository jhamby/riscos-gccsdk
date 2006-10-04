/* getpriority ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>
#include <sys/resource.h>
#include <unixlib/unix.h>
#include <limits.h>

/* Return the highest priority of any process specified by 'which' and 'who'
   (see <sys/resource.h>); if WHO is zero, the current process, process group,
   or user (as specified by WHO) is used.  A lower priority number means higher
   priority.  Priorities range from PRIO_MIN to PRIO_MAX.  */
int
getpriority (enum __priority_which which, id_t who)
{
  struct __sul_process *child;
  int ret = -1;

  if (who == 0)
    {
      /* Return priorities of the current process.  */
      switch (which)
	{
	case PRIO_PROCESS:
	  return __proc->ppri;
	case PRIO_PGRP:
	  return __proc->gpri;
	case PRIO_USER:
	  return __proc->upri;
	default:
	  errno = EINVAL;
	  return -1;
	}
      return -1;
    }

  child = __proc->children;
  while (child && ret == -1)
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
	  errno = EINVAL;
	  return -1;
	}
      child = child->next_child;
    }

  if (ret == -1)
    /* Invalid value of 'which'.  */
    errno = ESRCH;

  return ret;
}
