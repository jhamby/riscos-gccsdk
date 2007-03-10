/* pthread_testcancel
   Separate from cancel.c to avoid pulling in other cancel functions
   for a non threaded program
   Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers
   Written by Alex Waugh */


#include <unixlib/os.h>
#include <pthread.h>

/* Introduce a cancellation point into the current thread */
void
pthread_testcancel (void)
{
  if (__pthread_running_thread->cancelpending && __pthread_worksemaphore == 0)
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
