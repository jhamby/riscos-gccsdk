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

/* Context switching/schedulling */

/* Written by Martin Piper and Alex Waugh */


#include <stdlib.h>
#include <errno.h>
#include <unixlib/os.h>
#include <unixlib/unix.h> /* for __stackalloc */
#include <time.h>
#include <pthread.h>

pthread_t __pthread_thread_list = NULL; /* Linked list of all threads */
pthread_t __pthread_running_thread = NULL; /* Currently running thread */

int __pthread_running_threads = 0; /* Number of threads */


/* De-allocates an idle node from the list */
static void
__pthread_cleanup_idle (pthread_t node)
{
  if (node == NULL)
    return;

  if (node->state != STATE_IDLE)
    __pthread_fatal_error ("-- __pthread_cleanup_idle: Not an idle thread!");

#ifdef PTHREAD_DEBUG
  __os_print ("-- __pthread_cleanup_idle: Marking as STATE_UNALLOCED idle thread node ");
  __os_prhex ((int) node);
  __os_print (" from link list\r\n");
#endif
  node->state = STATE_UNALLOCED;

  if (node->saved_context)
    {
      free (node->saved_context);
      node->saved_context = NULL;
    }

  if (node->stack)
    {
      __free_stack_chain (node->stack);
      node->stack = NULL;
    }

  /* Free the node if it is detached */
  /* If not, then we can't do anything until another thread joins with it */
  if (node->detachstate == PTHREAD_CREATE_DETACHED)
    {
      node->magic = 0; /* Invalidate magic number */
      free (node);
    }
}

/* Round robin scheduller */
/* Occurs under a callback from the call_every interupt */
void
__pthread_context_switch (void)
{
  pthread_t next;
  int iter = 0;

#ifdef PTHREAD_DEBUG_CONTEXT
  __os_print ("-- __pthread_context_switch: __pthread_running_thread  = ");
  __os_prhex ((int) __pthread_running_thread);
  __os_print ("\r\n");
#endif

  next = __pthread_running_thread->next;

  /* Loop around the list looking for a thread that is running */
  do
    {
#ifdef PTHREAD_DEBUG_CONTEXT
/*      __os_print ("-- __pthread_context_switch: Looking for non idle thread\r\n");*/
#endif
      if (next != NULL)
        {
          __pthread_running_thread = next;
        }
      else
        {
          if (iter++ > 1)
            __pthread_fatal_error ("-- __pthread_context_switch: Deadlocked (no runnable threads to schedule)");

          __pthread_running_thread = __pthread_thread_list;
        }

      next = __pthread_running_thread->next;

      /* Free up any idle threads that are found */
      if (next != NULL && next->state == STATE_IDLE)
        {
          __pthread_running_thread->next = next->next;
          __pthread_cleanup_idle (next);
          next = __pthread_running_thread->next;
        }

      if (__pthread_running_thread->state == STATE_COND_TIMED_WAIT)
        {
          /* Check that the condition variable wait has not timed out */
          iter = 0; /* Prevent deadlock error ocurring if we are waiting */
          if (clock () > __pthread_running_thread->condtimeout)
            {
              /* Hmm, this will fail if the monotonic timer wraps around */
              __pthread_running_thread->state = STATE_RUNNING;
            }
        }
#ifdef PTHREAD_DEBUG_CONTEXT
      __os_print ("-- __pthread_context_switch: thread = ");
      __os_prhex ((int)__pthread_running_thread);
      __os_print (" State = ");
      __os_prdec (__pthread_running_thread->state);
      __os_print ("\r\n");
#endif

    }
  while (__pthread_running_thread->state < STATE_RUNNING);

  if (__pthread_running_thread->cancelpending && __pthread_running_thread->cancelstate == PTHREAD_CANCEL_ENABLE && __pthread_running_thread->canceltype == PTHREAD_CANCEL_ASYNCHRONOUS)
    {
      /* Asynchronously cancel the thread */
      __pthread_running_thread->saved_context->r[15] = (int)pthread_testcancel;
    }

#ifdef PTHREAD_DEBUG_CONTEXT
  __os_print ("-- __pthread_context_switch: New __pthread_running_thread  = ");
  __os_prhex ((int) __pthread_running_thread);
  __os_print ("\r\n");
#endif
}

