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

/* Thread specific keys */

/* Written by Alex Waugh */

#include <errno.h>
#include <stdlib.h>
#include <pthread.h>


static pthread_key_t nextkey = 0; /* ID to allocate to next new key */

static struct __pthread_key *keys = NULL; /* Linked list of all keys */


/* Create a key and associate a destructor function with it */
int
pthread_key_create (pthread_key_t *key, void (*destructor) (void*))
{
  struct __pthread_key *newkey;

  if (key == NULL)
    return EINVAL;

  newkey = malloc (sizeof (struct __pthread_key));
  if (newkey == NULL)
    return ENOMEM;

  __pthread_disable_ints ();

  if (nextkey >= PTHREAD_KEYS_MAX)
    {
      __pthread_enable_ints ();
      return EAGAIN;
    }

  newkey->next = keys;
  keys = newkey;
  newkey->keyid = nextkey++;
  newkey->destructor = destructor;

  *key = newkey->keyid;

  __pthread_enable_ints ();
  return 0;
}

/* Deletes a thread specific key (does not call the key's destructors) */
int
pthread_key_delete (pthread_key_t key)
{
  struct __pthread_key *keylist;

  if (keys == NULL)
    return EINVAL;

  __pthread_disable_ints ();

  /* Remove key from the linked list */
  if (keys->keyid == key)
    {
      keylist = keys->next;
      free (keys);
      keys = keylist;
    }
  else
    {
      struct __pthread_key *temp;

      keylist = keys;
      while (keylist->next && keylist->next->keyid != key)
        keylist = keylist->next;

      if (keylist->next == NULL)
        {
          __pthread_enable_ints ();
          return EINVAL;
        }

      temp = keylist->next;
      keylist->next = temp->next;
      free (temp);
    }

  __pthread_enable_ints ();
  return 0;
}

/* Set a thread specific value to associate with the given key */
int
pthread_setspecific (pthread_key_t key, const void *value)
{
  struct __pthread_key *keylist;
  struct __pthread_key *newkey;

  __pthread_disable_ints ();

  /* Find key in linked list */
  keylist = keys;
  while (keylist != NULL && keylist->keyid != key)
    keylist = keylist->next;

  if (keylist == NULL)
    {
      __pthread_enable_ints ();
      return EINVAL;
    }

  newkey = malloc (sizeof (struct __pthread_key));
  if (newkey == NULL)
    {
      __pthread_enable_ints ();
      return ENOMEM;
    }

  newkey->keyid = key;
  newkey->value = (void *)value;
  newkey->destructor = keylist->destructor;

  /* Add key to list for this thread */
  newkey->next = __pthread_running_thread->keys;
  __pthread_running_thread->keys = newkey;

  __pthread_enable_ints ();
  return 0;
}

/* Read value associated with given key for the current thread */
/* Returns NULL if no value has been set for this thread yet */
void *
pthread_getspecific (pthread_key_t key)
{
  struct __pthread_key *keylist;

  __pthread_disable_ints ();
  keylist = __pthread_running_thread->keys;
  while (keylist != NULL)
    {
      if (keylist->keyid == key)
        {
          __pthread_enable_ints ();
          return keylist->value;
        }

      keylist = keylist->next;
    }

  __pthread_enable_ints ();
  return NULL;
}

