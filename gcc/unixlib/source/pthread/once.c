/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/once.c,v $
 * $Date: 2002/12/15 13:16:55 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: once.c,v 1.1 2002/12/15 13:16:55 admin Exp $";
#endif

/* Written by Alex Waugh */

#include <errno.h>
#include <pthread.h>

static pthread_mutex_t once_mutex = PTHREAD_MUTEX_INITIALIZER;


/* Cleanup handler for pthread_once */
static void
__pthread_once_cleanup (void *arg)
{
  arg = arg;
  pthread_mutex_unlock (&once_mutex);
}

/* Ensure a specified function is called exactly once
   We need a mutex so that only one call to pthread_once
   can proceed at any one time, and we can't have ints
   disabled during the init_routine call.
   If the init_routine is cancelled then the effect should
   be as if pthread_once was never called. */
int
pthread_once (pthread_once_t *once_control, void (*init_routine) (void))
{
  if (once_control == NULL || init_routine == NULL)
    return EINVAL;

  pthread_mutex_lock (&once_mutex); /* This won't fail */

  if (*once_control == 0)
    {
      pthread_cleanup_push (__pthread_once_cleanup, NULL);

      init_routine ();

      pthread_cleanup_pop (0);

      *once_control = 1;
    }

  pthread_mutex_unlock (&once_mutex);

  return 0;
}
