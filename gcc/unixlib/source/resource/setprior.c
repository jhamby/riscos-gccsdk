/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/resource/setprior.c,v $
 * $Date: 2004/06/12 08:59:48 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: setprior.c,v 1.5 2004/06/12 08:59:48 peter Exp $";
#endif

#include <errno.h>
#include <sys/resource.h>
#include <unixlib/unix.h>
#include <limits.h>
#include <pthread.h>

/* A return value of zero indicates success.  */

int
setpriority (enum __priority_which which, int who, int prio)
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
              if (child->euid == (unsigned int)who)
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
