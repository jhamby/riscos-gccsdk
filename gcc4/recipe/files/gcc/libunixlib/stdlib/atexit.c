/* atexit (), __cxa_atexit (), __cxa_finalize ()
 * Copyright (c) 2000-2015 UnixLib Developers
 */

#include <errno.h>
#include <stdlib.h>

#include <pthread.h>
#include <internal/unix.h>
#include <internal/linklist.h>

typedef struct atexit_entry
{
  __link_header link;

  void (*func)(void *);
  void *arg;
  void *dso_handle;
  unsigned called : 1;
} atexit_entry;

static __link_list atexit_function_list;

int
__cxa_atexit(void (*destructor)(void *),
	     void *arg,
	     void *dso_handle)
{
  PTHREAD_UNSAFE

  if (destructor == NULL)
    return __set_errno (EINVAL);

  atexit_entry *entry = malloc (sizeof (atexit_entry));
  if (entry == NULL)
    return __set_errno (ENOMEM);

  __linklist_init_link (&entry->link);
  __linklist_add_to_front (&atexit_function_list, &entry->link);

  entry->func = destructor;
  entry->arg = arg;
  entry->dso_handle = dso_handle;
  entry->called = 0;

  return 0;
}

/* When __cxa_finalize(d) is called, it shall walk the termination function list,
 * calling each in turn if d matches the handle of the termination function entry.
 * If d is NULL, it shall call all the termination funtions.
 */
void
__cxa_finalize(void *dso_handle)
{
  PTHREAD_UNSAFE

  atexit_entry *entry = (atexit_entry *)__linklist_first (&atexit_function_list);

  /* __cxa_atexit adds handlers to the front of the list, so running through
   * it forwards here means that we process the handlers in reverse order as
   * the standard requires.
   * Rather than remove the entries once called, they are simply marked as
   * finished. __cxa_finalize can be called recursively, so removing links
   * from the list can be problematic (the next entry may no longer exist).
   * As __cxa_finalize is called at program exit, the memory loss shouldn't
   * be an issue.
   */
  while (entry)
    {
      atexit_entry *next = (atexit_entry *)__linklist_next (&entry->link);
      if (!entry->called && (dso_handle == NULL || dso_handle == entry->dso_handle))
        {
	  entry->called = 1;
	  __funcall ((*entry->func), (entry->arg));
	}
      entry = next;
    }
}

int
atexit (void (*atexit_function) (void))
{
  return __cxa_atexit ((void (*) (void *))atexit_function, NULL, NULL);
}
