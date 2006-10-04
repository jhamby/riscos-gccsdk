/* setpriority ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <errno.h>
#include <sys/resource.h>
#include <unixlib/unix.h>
#include <limits.h>
#include <pthread.h>

/* A return value of zero indicates success.  */

int
setpriority (enum __priority_which which, id_t who, int prio)
{
  struct __sul_process *process = NULL;

  PTHREAD_UNSAFE

  if (who == 0)
    {
      /* Set the priority of the current process.  */
      process = __proc;
    }
  else
    {
      struct __sul_process *child = __proc->children;
      while (child && process == NULL)
        {
          switch (which)
            {
            case PRIO_PROCESS:
              if (child->pid == who)
                process = child;
              break;
            case PRIO_PGRP:
              if (child->pgrp == who)
                process = child;
              break;
            case PRIO_USER:
              if (child->euid == who)
                process = child;
              break;
            default:
              errno = EINVAL;
              return -1;
            }
          child = child->next_child;
        }
    }

  if (process == NULL)
    {
      errno = ESRCH;
      return -1;
    }

  switch (which)
    {
    case PRIO_PROCESS:
      if (process->gpri > prio)
        {
          errno = EACCES;
          return -1;
        }
      process->ppri = (prio > PRIO_MAX) ? PRIO_MAX
                    : (prio < PRIO_MIN) ? PRIO_MIN : prio;
      break;
    case PRIO_PGRP:
      if (process->gpri > prio)
        {
          errno = EACCES;
          return -1;
        }
      process->gpri = (prio > PRIO_MAX) ? PRIO_MAX
                    : (prio < PRIO_MIN) ? PRIO_MIN : prio;
      break;
    case PRIO_USER:
      if (process->upri > prio)
        {
          errno = EACCES;
          return -1;
        }
      process->upri = (prio > PRIO_MAX) ? PRIO_MAX
                    : (prio < PRIO_MIN) ? PRIO_MIN : prio;
      break;
    default:
      errno = EINVAL;
      return -1;
    }

  return 0;
}
