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

/* Written by Alex Waugh */

#include <pthread.h>
#include <errno.h>
#include <stdlib.h>

/* Detach a thread. No thread should subsequently try to join with this thread */
int
pthread_detach (pthread_t thread)
{
  if (__pthread_invalid (thread))
    return EINVAL;

  __pthread_disable_ints ();

  thread->detachstate = PTHREAD_CREATE_DETACHED;
  /* If the thread has already terminated then free any remaining storage */
  /* It will have already have been removed from the list of running threads */
  if (thread->state == STATE_UNALLOCED)
    {
      thread->magic = 0;
      free (thread);
    }

  __pthread_enable_ints ();
  return 0;
}
