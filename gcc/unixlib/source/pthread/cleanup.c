/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/cleanup.c,v $
 * $Date: 2002/12/15 13:16:55 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: cleanup.c,v 1.1 2002/12/15 13:16:55 admin Exp $";
#endif

/* Push and pop thread cleanup handlers, to be called when a thread exits */

/* Written by Alex Waugh */

#include <pthread.h>

void
__pthread_cleanup_push (struct __pthread_cleanup *cleanup)
{
  __pthread_disable_ints ();

  cleanup->next = __pthread_running_thread->cleanupfns;
  __pthread_running_thread->cleanupfns = cleanup;

  __pthread_enable_ints ();
}

void
__pthread_cleanup_pop (void)
{
  __pthread_disable_ints ();

  __pthread_running_thread->cleanupfns = __pthread_running_thread->cleanupfns->next;

  __pthread_enable_ints ();
}

