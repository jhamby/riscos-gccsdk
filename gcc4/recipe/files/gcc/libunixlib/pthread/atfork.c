/* Register handlers to be called at fork time.
   Copyright (c) 2002, 2003, 2004, 2005, 2006, 2008 UnixLib Developers.
   Written by Alex Waugh.  */

#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <malloc.h>

#include <internal/unix.h>

struct fork_handlers
{
  void (*prepare)(void);
  void (*parent)(void);
  void (*child)(void);
  struct fork_handlers *next;
};

/* Linked list of all registered handlers */
static struct fork_handlers *handlers = NULL;

/* Register handlers to be called immediatly before and after a
   fork occurs.  */
int
pthread_atfork (void (*prepare)(void), void (*parent)(void),
		void (*child)(void))
{
  struct fork_handlers *newhandlers;
  int retval;
  struct ul_global *gbl = &__ul_global;

  if (! gbl->pthread_system_running)
    gbl->pthread_system_running = 1;

  __pthread_disable_ints ();
  newhandlers = malloc_unlocked (gbl->malloc_state,
				 sizeof (struct fork_handlers));
  if (newhandlers == NULL)
    retval = ENOMEM;
  else
    {
      newhandlers->prepare = prepare;
      newhandlers->parent = parent;
      newhandlers->child = child;
      newhandlers->next = handlers;
      handlers = newhandlers;
      retval = 0;
    }
  __pthread_enable_ints ();
  return retval;
}

/* Called by the fork code just before the fork takes place.
   Prepare handlers are called in the opposite order to which they
   were registered.  */
void
__pthread_atfork_callprepare (void)
{
  struct fork_handlers *handler = handlers;

  while (handler != NULL)
    {
      if (handler->prepare != NULL)
        handler->prepare ();

      handler = handler->next;
    }
}

/* Call handlers in the same order that they were registered */
static void
__pthread_atfork_recurse (const int parent,
			  const struct fork_handlers *handler)
{
  if (handler != NULL)
    {
      __pthread_atfork_recurse (parent, handler->next);

      if (parent)
        {
          if (handler->parent != NULL)
            handler->parent ();
        }
      else
        {
          if (handler->child != NULL)
            handler->child ();
        }
    }
}

/* Called by the fork code just after the fork has taken place.
   Parent is nonzero if this is being called from the parent process.  */
void
__pthread_atfork_callparentchild (const int parent)
{
  __pthread_atfork_recurse (parent, handlers);
}

