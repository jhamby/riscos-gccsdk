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

