/* pthread_testcancel
   Separate from cancel.c to avoid pulling in other cancel functions for a non
   threaded program.
   Written by Alex Waugh.
   Copyright (c) 2002-2008 UnixLib Developers.  */

#include <pthread.h>

#include <internal/os.h>
#include <internal/unix.h>

/* Introduce a cancellation point into the current thread.  */
void
pthread_testcancel (void)
{
  struct ul_global *gbl = &__ul_global;

  if (__pthread_running_thread->cancelpending
      && gbl->pthread_worksemaphore == 0)
    {
      if (__pthread_running_thread->cancelstate == PTHREAD_CANCEL_ENABLE
          && __pthread_running_thread->state == STATE_RUNNING)
        {
#ifdef PTHREAD_DEBUG
          __os_print("-- pthread_testcancel: thread cancelled\r\n");
#endif
          pthread_exit(PTHREAD_CANCELED);
        }
    }

#ifdef PTHREAD_DEBUG
  __os_print("-- pthread_testcancel: thread not cancelled\r\n");
#endif
}
