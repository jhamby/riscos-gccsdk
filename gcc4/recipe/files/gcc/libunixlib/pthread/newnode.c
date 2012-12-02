/* Internal thread node creation.
   Written by Martin Piper and Alex Waugh.
   Copyright (c) 2002-2012 UnixLib Developers.  */

#include <stdlib.h>
#include <errno.h>
#include <malloc.h>
#include <pthread.h>

#include <internal/os.h>
#include <internal/unix.h>

/* #define DEBUG_PTHREAD */
#ifdef DEBUG_PTHREAD
#  define DEBUG
#  include <sys/debug.h>
#endif

/* Initialise an new thread node to hold all the details of the thread.
   If node is NULL, then allocate memory for the node as well.

   It is important that memory allocated for the new node is done via
   the SUL malloc routine as race conditions can occur between the
   userland malloc functions and context.c::__pthread_cleanup_idle.

   The particular instance that inspired this change was a context switch
   that was occurring during a malloc operation, so malloc's mutex was
   claimed.  The idle cleanup function was deleting a pthread structure
   and blocked on the call to 'free'.  */
pthread_t
__pthread_new_node (pthread_t node)
{
  if (node == NULL)
    {
      struct ul_global *gbl = &__ul_global;
      node = malloc_unlocked (gbl->malloc_state,
			      sizeof (struct __pthread_thread));
      if (node == NULL)
        {
#ifdef DEBUG_PTHREAD
          debug_printf ("-- __pthread_new_node: Unable to allocate thread node\n");
#endif
          return NULL;
        }

      node->saved_context =
	malloc_unlocked (gbl->malloc_state,
			 sizeof (struct __pthread_saved_context));
      if (node->saved_context == NULL)
        {
#ifdef DEBUG_PTHREAD
          debug_printf ("-- __pthread_new_node: Unable to allocate thread saved_context\n");
#endif
          free_unlocked (gbl->malloc_state, node);
          return NULL;
        }
    }

  /* Ensure the magic number is invalid until the thread is ready to run.  */
  node->magic = 0;
  node->alloca = NULL;
  node->thread_errno = 0;
  node->errbuf.errnum = 0;
  node->errbuf.errmess[0] = '\0';
  node->errbuf_valid = '\0';
  node->next = NULL;
  node->stack = NULL;
  node->keys = NULL;
  node->cleanupfns = NULL;
  node->state = STATE_RUNNING;
  node->cancelstate = PTHREAD_CANCEL_ENABLE;
  node->canceltype = PTHREAD_CANCEL_DEFERRED;
  node->cancelpending = 0;
  node->detachstate = PTHREAD_CREATE_JOINABLE;
  node->joined = NULL;
  node->ret = NULL;
  node->mutex = NULL;
  node->cond = NULL;
  node->nextwait = NULL;
  pthread_cond_init (&(node->sigwait_cond), NULL);
  sigemptyset (&(node->pending));
  sigemptyset (&(node->blocked));

  return node;
}

/* Allocate and initialise a new stack chunk */
struct __stack_chunk *
__pthread_new_stack (void)
{
#if __UNIXLIB_CHUNKED_STACK
  struct __stack_chunk *stack = __stackalloc (PTHREAD_STACK_MIN);
  if (stack == NULL)
    {
#ifdef DEBUG_PTHREAD
      debug_printf ("-- __pthread_new_stack: Unable to allocate thread stack\n");
#endif
      return NULL;
    }

  stack->magic = STACK_CHUNK_MAGIC;
  stack->next = NULL ;
  stack->prev = NULL ;
  stack->size = PTHREAD_STACK_MIN;
  stack->dealloc = NULL;
  stack->returnaddr = NULL;

  return stack;
#else
  return NULL; /* FIXME: code missing.  */
#endif
}
