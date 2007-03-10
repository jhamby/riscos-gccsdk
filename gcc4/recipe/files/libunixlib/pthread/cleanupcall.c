/* Thread cleanup handlers, called when a thread exits.
   Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers
   Written by Alex Waugh */

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

