/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/exit.c,v $
 * $Date: 2003/04/06 10:58:41 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: exit.c,v 1.3 2003/04/06 10:58:41 alex Exp $";
#endif

/* Written by Martin Piper and Alex Waugh */

#include <stdlib.h>
#include <errno.h>
#include <unixlib/os.h>
#include <pthread.h>


/* Exit from the current thread, or exit the entire program if there is only a single thread */
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

  /* Call thread specific destructors/cleanup handlers */
  /* Context switching may take place whilst these are running */
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
      __pthread_running_thread->detachstate = PTHREAD_CREATE_DETACHED; /* We have joined, so treat the thread as detached */
    }

  thread = __pthread_running_thread;

  __pthread_exit ();

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
      /* There is no need for the ticker if there is only one thread left */
      __pthread_stop_ticker ();
    }

  if (__pthread_num_running_threads > 0)
    {
      /* Not the last thread */
      __pthread_enable_ints ();

      while (1)
        {
          /* The context switcher will deallocate all storage for this thread when it notices it is idle */
          thread->state = STATE_IDLE;
          pthread_yield ();
        }
    }

#ifdef PTHREAD_DEBUG
  __os_print ("-- pthread_exit: Last thread exited, calling exit()\r\n");
#endif

  exit ((int)status);
}

