/****************************************************************************
 *
 * $Source: $
 * $Date: $
 * $Revision: $
 * $State: $
 * $Author: $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: $";
#endif

/* Written by Alex Waugh */

#include <sys/types.h>
#include <signal.h>
#include <unixlib/unix.h>
#include <pthread.h>

/* Send a signal to a thread */
int
pthread_kill (pthread_t thread, int sig)
{
  thread = thread;
  return kill (__u->pid, sig);
}

/* Alter a thread's signal mask */
int
pthread_sigmask (int how, const sigset_t *set, sigset_t *oset)
{
  return sigprocmask (how, set, oset);
}
