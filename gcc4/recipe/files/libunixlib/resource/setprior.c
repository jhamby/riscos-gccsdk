/* setpriority ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <sys/resource.h>
#include <limits.h>

#include <pthread.h>
#include <internal/unix.h>

/* A return value of zero indicates success.  */

int
setpriority (enum __priority_which which, id_t who, int prio)
{
  struct __sul_process *sulproc;

  PTHREAD_UNSAFE

  if (who == 0)
    {
      /* Set the priority of the current process.  */
      sulproc = __ul_global.sulproc;
    }
  else
    {
      struct __sul_process *child;

      for (child = __ul_global.sulproc->children, sulproc = NULL;
	   child && sulproc == NULL;
	   child = child->next_child)
        {
          switch (which)
            {
            case PRIO_PROCESS:
              if (child->pid == who)
                sulproc = child;
              break;
            case PRIO_PGRP:
              if (child->pgrp == who)
                sulproc = child;
              break;
            case PRIO_USER:
              if (child->euid == who)
                sulproc = child;
              break;
            default:
              return __set_errno (EINVAL);
            }
        }
    }

  if (sulproc == NULL)
    return __set_errno (ESRCH);

  switch (which)
    {
    case PRIO_PROCESS:
      if (sulproc->gpri > prio)
        return __set_errno (EACCES);
      sulproc->ppri = (prio > PRIO_MAX) ? PRIO_MAX
                        : (prio < PRIO_MIN) ? PRIO_MIN : prio;
      break;
    case PRIO_PGRP:
      if (sulproc->gpri > prio)
        return __set_errno (EACCES);
      sulproc->gpri = (prio > PRIO_MAX) ? PRIO_MAX
                        : (prio < PRIO_MIN) ? PRIO_MIN : prio;
      break;
    case PRIO_USER:
      if (sulproc->upri > prio)
        return __set_errno (EACCES);
      sulproc->upri = (prio > PRIO_MAX) ? PRIO_MAX
                        : (prio < PRIO_MIN) ? PRIO_MIN : prio;
      break;
    default:
      return __set_errno (EINVAL);
    }

  return 0;
}
