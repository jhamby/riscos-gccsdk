/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/signal.c,v $
 * $Date: 2005/02/09 21:13:40 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: signal.c,v 1.2 2005/02/09 21:13:40 alex Exp $";
#endif

/* Written by Alex Waugh */

#include <sys/types.h>
#include <signal.h>
#include <unixlib/unix.h>
#include <pthread.h>
#include <strings.h>
#include <errno.h>

static pthread_mutex_t sigwait_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Send a signal to a thread */
int
pthread_kill (pthread_t thread, int sig)
{
  pthread_mutex_lock (&sigwait_mutex);

  sigaddset (&(thread->pending), sig);

  pthread_mutex_unlock (&sigwait_mutex);

  return pthread_cond_signal (&(thread->sigwait_cond));
}

int
sigwait (const sigset_t *set, int *sig)
{
  sigset_t pending;
  int found;

  pthread_mutex_lock (&sigwait_mutex);

  do
    {
      pending = *set & __pthread_running_thread->pending;
      found = ffs (pending);

      if (found == 0)
        {
          pthread_cond_wait (&(__pthread_running_thread->sigwait_cond), &sigwait_mutex);
        }
    }
  while (found == 0);

  sigdelset (&(__pthread_running_thread->pending), found);

  if (sig)
    *sig = found;

  pthread_mutex_unlock (&sigwait_mutex);

  return 0;
}

/* Alter a thread's signal mask */
int
pthread_sigmask (int how, const sigset_t *nset, sigset_t *oset)
{
  sigset_t mask;

  if (oset != NULL)
    *oset = __pthread_running_thread->blocked;

  if (nset == NULL)
    return 0;

  mask = (unsigned int) *nset;

  if (how == SIG_BLOCK)
    {
      __pthread_running_thread->blocked |= mask;
    }
  else if (how == SIG_UNBLOCK)
    {
      __pthread_running_thread->blocked &= ~mask;
      /* At this point we should raise any pending signals that are unmasked,
         but as the main signal code is not yet thread aware then don't bother */
    }
  else if (how == SIG_SETMASK)
    {
      __pthread_running_thread->blocked = mask;
      /* At this point we should raise any pending signals that are unmasked,
         but as the main signal code is not yet thread aware then don't bother */
    }
  else
    {
      return __set_errno (EINVAL);
    }
  return 0;
}
