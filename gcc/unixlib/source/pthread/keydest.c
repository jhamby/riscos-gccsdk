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

#include <stdlib.h>
#include <pthread.h>

/* Separated from key.c to avoid other key functions being
   pulled in by the linker */

/* Call all the destructors of a thread specific key */
void
__pthread_key_calldestructors (void)
{
  struct __pthread_key *key;
  int trys = 0;

  while ((key = __pthread_running_thread->keys) != NULL)
    {
      __pthread_running_thread->keys = key->next;
      if (key->value != NULL && key->destructor != NULL && trys < PTHREAD_DESTRUCTOR_ITERATIONS)
        {
          trys++;
          key->destructor (key->value);
        }
      free (key);
    }
}
