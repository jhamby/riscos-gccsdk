/* Push and pop thread cleanup handlers, to be called when a thread exits.
   Written by Alex Waugh.
   Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers.  */

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
