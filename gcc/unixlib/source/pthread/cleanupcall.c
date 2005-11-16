/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/cleanupcall.c,v $
 * $Date: 2002/12/15 13:16:55 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* Thread cleanup handlers, called when a thread exits */

/* Written by Alex Waugh */

#include <pthread.h>

void
__pthread_cleanup_callhandlers (void)
{
  struct __pthread_cleanup *cleanup = __pthread_running_thread->cleanupfns;

  while (cleanup != NULL)
    {
      if (cleanup->routine != NULL)
        cleanup->routine(cleanup->arg);

      cleanup = cleanup->next;
    }
}

