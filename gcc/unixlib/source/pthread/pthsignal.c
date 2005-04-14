/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/pthsignal.c,v $
 * $Date: 2005/04/14 12:13:09 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: pthsignal.c,v 1.1 2005/04/14 12:13:09 nick Exp $";
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

  PTHREAD_SAFE_CANCELLATION

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
