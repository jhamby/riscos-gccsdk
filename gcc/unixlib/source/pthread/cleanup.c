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

/* Push and pop thread cleanup handlers, to be called when a thread exits */

/* Written by Alex Waugh */

#include <pthread.h>

void
__pthread_cleanup_push (struct __pthread_cleanup *cleanup)
{
  PTHREAD_INIT

  __pthread_disable_ints ();

  cleanup->next = __pthread_running_thread->cleanupfns;
  __pthread_running_thread->cleanupfns = cleanup;

  __pthread_enable_ints ();
}

void
__pthread_cleanup_pop (void)
{
  PTHREAD_INIT

  __pthread_disable_ints ();

  __pthread_running_thread->cleanupfns = __pthread_running_thread->cleanupfns->next;

  __pthread_enable_ints ();
}

