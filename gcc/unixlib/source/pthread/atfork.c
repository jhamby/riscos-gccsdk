/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/atfork.c,v $
 * $Date: 2002/12/15 13:16:55 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: atfork.c,v 1.1 2002/12/15 13:16:55 admin Exp $";
#endif

/* Written by Alex Waugh */

#include <pthread.h>
#include <errno.h>
#include <stdlib.h>

struct fork_handlers
{
  void (*prepare)(void);
  void (*parent)(void);
  void (*child)(void);
  struct fork_handlers *next;
};

static struct fork_handlers *handlers = NULL; /* Linked list of all registered handlers */

/* Register handlers to be called immediatly before and after a fork occurs */
int
pthread_atfork (void (*prepare)(void), void (*parent)(void), void (*child)(void))
{
  struct fork_handlers *newhandlers;

  newhandlers = malloc (sizeof (struct fork_handlers));
  if (newhandlers == NULL)
    return ENOMEM;

  __pthread_disable_ints ();

  newhandlers->prepare = prepare;
  newhandlers->parent = parent;
  newhandlers->child = child;
  newhandlers->next = handlers;
  handlers = newhandlers;

  __pthread_enable_ints ();

  return 0;
}

/* Called by the fork code just before the fork takes place */
/* Prepare handlers are called in the opposite order to which they were registered */
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
__pthread_atfork_recurse (const int parent, const struct fork_handlers *handler)
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

/* Called by the fork code just after the fork has taken place */
/* Parent is nonzero if this is being called from the parent process */
void
__pthread_atfork_callparentchild (const int parent)
{
  __pthread_atfork_recurse (parent, handlers);
}

