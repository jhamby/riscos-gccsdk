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

/* Return the thread id of the currently running thread */
/* Will start the thread system if it's not running */
pthread_t
pthread_self (void)
{
  PTHREAD_INIT

  return __pthread_running_thread;
}
