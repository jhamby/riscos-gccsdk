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

/* Written by Martin Piper and Alex Waugh */

#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <unixlib/os.h>

/* Wait for the specified thread to finish, and read its exit value */
int
pthread_join (pthread_t tojoin, void **status)
{
  pthread_t thread;

  PTHREAD_INIT

  if (tojoin == __pthread_running_thread)
    {
#ifdef PTHREAD_DEBUG
      __os_print ("-- pthread_join: Target thread is the same as calling thread\r\n");
#endif
      return EDEADLK;
    }

  __pthread_disable_ints ();

  if (__pthread_invalid (tojoin))
    {
#ifdef PTHREAD_DEBUG
      __os_print ("-- pthread_join: Target thread does not exist\r\n");
#endif
      __pthread_enable_ints ();
      return ESRCH;
    }


  thread = __pthread_running_thread;

  if (tojoin->joined != NULL || tojoin->detachstate == PTHREAD_CREATE_DETACHED)
    {
#ifdef PTHREAD_DEBUG
      __os_print ("-- pthread_join: Target thread is already joined or detached\r\n");
#endif
      __pthread_enable_ints ();
      return EINVAL;
    }

  switch (tojoin->state)
    {
      case STATE_UNALLOCED:
#ifdef PTHREAD_DEBUG
        __os_print ("-- pthread_join: Joining with unalloced thread\r\n");
#endif
        if (status != NULL)
          *status = tojoin->ret;

        tojoin->magic = 0;
        free (tojoin);

        __pthread_enable_ints ();
        break;

      case STATE_IDLE:
#ifdef PTHREAD_DEBUG
        __os_print ("-- pthread_join: Joining with idle thread\r\n");
#endif
        if (status != NULL)
          *status = tojoin->ret;

        __pthread_enable_ints ();
        break;

      default:
#ifdef PTHREAD_DEBUG
        __os_print ("-- pthread_join: Suspending thread\r\n");
#endif
        /* Suspend the running thread and mark it as joined to another */
        thread->state = STATE_JOIN;
        /* Join the thread */
        tojoin->joined = thread;

        __pthread_enable_ints ();
        pthread_yield (); /* Will not return until the other thread has exited */

        if (status != NULL)
          *status = __pthread_running_thread->ret;
    }

  return 0;
}

