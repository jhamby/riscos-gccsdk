/* Internal thread node creation.
   Copyright (c) 2004, 2005 UnixLib Developers.
   Written by Martin Piper and Alex Waugh.  */

#include <stdlib.h>
#include <errno.h>
#include <unixlib/os.h>
#include <unixlib/unix.h>
#include <pthread.h>


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
      node = __proc->sul_malloc (__proc->pid,
				 sizeof (struct __pthread_thread));
      if (node == NULL)
        {
#ifdef DEBUG_PTHREAD
          __os_print("-- __pthread_new_node: Unable to allocate thread node\r\n");
#endif
          return NULL;
        }

      node->saved_context =
	__proc->sul_malloc (__proc->pid,
			    sizeof (struct __pthread_saved_context));
      if (node->saved_context == NULL)
        {
#ifdef DEBUG_PTHREAD
          __os_print ("-- _pthread_new_node: Unable to allocate thread saved_context\r\n");
#endif
          __proc->sul_free (__proc->pid, node);
          return NULL;
        }
    }

  /* Ensure the magic number is invalid until the thread is ready to run.  */
  node->magic = 0;
  node->alloca[0] = 0;
  node->alloca[1] = 0;
  node->alloca[2] = 0;
  node->thread_errno = 0;
  node->errbuf.errnum = 0;
  node->errbuf.errmess[0] = '\0';
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
  struct __stack_chunk *stack;

  stack = __stackalloc (PTHREAD_STACK_MIN);
  if (stack == NULL)
    {
#ifdef DEBUG_PTHREAD
      __os_print("-- __pthread_new_stack: Unable to allocate thread stack\r\n");
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
}

