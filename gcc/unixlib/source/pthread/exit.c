/* Terminate the calling thread.
   Copyright (c) 2003, 2004, 2005 UnixLib Developers.
   Written by Martin Piper and Alex Waugh.  */

#include <stdlib.h>
#include <errno.h>
#include <unixlib/os.h>
#include <pthread.h>


/* Exit from the current thread, or exit the entire program if there
   is only a single thread.  */
void
pthread_exit (void *status)
{
  pthread_t thread;

  /* If we aren't running the thread system then just exit */
  if (!__pthread_system_running)
    exit ((int)status);

  __pthread_disable_ints ();

#ifdef PTHREAD_DEBUG
  __os_print ("-- pthread_exit(");
  __os_prhex ((int)status);
  __os_print ("): called from thread ");
  __os_prhex ((int)__pthread_running_thread);
  __os_nl ();
#endif

  __pthread_running_thread->state = STATE_CLEANUP;
  __pthread_enable_ints ();

  /* Call thread specific destructors/cleanup handlers.
     Context switching may take place whilst these are running.  */
  __pthread_cleanup_callhandlers ();
  __pthread_key_calldestructors ();

  __pthread_disable_ints ();

  /* Save return value */
  __pthread_running_thread->ret = status;

  /* Sort out the joined thread */
  if (__pthread_running_thread->joined)
    {
      thread = __pthread_running_thread->joined;
      thread->ret = status;
      thread->state = STATE_RUNNING;
      __pthread_running_thread->joined = NULL;

      /* We have joined, so treat the thread as detached.  */
      __pthread_running_thread->detachstate = PTHREAD_CREATE_DETACHED;
    }

  thread = __pthread_running_thread;

  /* We have to re-enable interrupts because during pthread_exit we
     potentially can call 'free' to tear-down any memory chunks allocated
     with 'alloca'.  */
  __pthread_enable_ints ();
  __pthread_exit ();
  __pthread_disable_ints ();

#ifdef PTHREAD_DEBUG
  __os_print ("-- pthread_exit: Thread ");
  __os_prhex ((int)thread);
  __os_print (" now exit idle\r\n");
#endif

  __pthread_num_running_threads--;

  if (__pthread_num_running_threads <= 1)
    {
#ifdef PTHREAD_DEBUG
      __os_print ("-- pthread_exit: Last or penultimate thread exited, stopping interrupts\r\n");
#endif
      /* There is no need for the ticker if there is only one thread left.  */
      __pthread_stop_ticker ();
    }

  if (__pthread_num_running_threads > 0)
    {
      /* Not the last thread */
      thread->state = STATE_IDLE;
      __pthread_enable_ints ();

      while (1)
        {
          /* The context switcher will deallocate all storage for
	     this thread when it notices it is idle.  */
          thread->state = STATE_IDLE;
          pthread_yield ();
        }
    }

#ifdef PTHREAD_DEBUG
  __os_print ("-- pthread_exit: Last thread exited, calling exit()\r\n");
#endif

  exit ((int)status);
}

