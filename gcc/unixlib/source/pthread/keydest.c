/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/keydest.c,v $
 * $Date: 2003/05/07 22:10:27 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: keydest.c,v 1.2 2003/05/07 22:10:27 alex Exp $";
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
  int tries = 0;

  while ((key = __pthread_running_thread->keys) != NULL)
    {
      __pthread_running_thread->keys = key->next;
      if (key->value.nonconst != NULL && key->destructor != NULL && tries < PTHREAD_DESTRUCTOR_ITERATIONS)
        {
          tries++;
          key->destructor (key->value.nonconst);
        }
      free (key);
    }
}
