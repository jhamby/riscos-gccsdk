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

/* pthread_testcancel
   Separate from cancel.c to avoid pulling in other cancel functions
   for a non threaded program */

/* Written by Alex Waugh */


#include <unixlib/os.h>
#include <pthread.h>

/* Introduce a cancellation point into the current thread */
void
pthread_testcancel (void)
{
  if (__pthread_running_thread->cancelpending)
    {
      if (__pthread_running_thread->cancelstate == PTHREAD_CANCEL_ENABLE && __pthread_running_thread->state == STATE_RUNNING)
        {
#ifdef PTHREAD_DEBUG
          __os_print("-- pthread_testcancel: thread cancelled\n\r");
#endif
          pthread_exit(PTHREAD_CANCELED);
        }
    }

#ifdef PTHREAD_DEBUG
  __os_print("-- pthread_testcancel: thread not cancelled\n\r");
#endif
}

