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

#include <stdlib.h>
#include <errno.h>
#include <unixlib/os.h>
#include <unixlib/unix.h>
#include <pthread.h>


/* Initialise an new thread node to hold all the details of the thread */
/* If node is NULL, then allocate memory for the node as well */
pthread_t
__pthread_new_node (pthread_t node)
{

  if (node == NULL)
    {
      node = malloc (sizeof (struct __pthread_thread));
      if (node == NULL)
        {
#ifdef DEBUG_PTHREAD
          __os_print("-- __pthread_new_node: Unable to allocate thread node\r\n");
#endif
          return NULL;
        }

      node->saved_context = malloc (sizeof (struct __pthread_saved_context));
      if (node->saved_context == NULL)
        {
#ifdef DEBUG_PTHREAD
          __os_print ("-- _pthread_new_node: Unable to allocate thread saved_context\r\n");
#endif
          free (node);
          return NULL;
        }
    }

  node->magic = 0; /* Ensure the magic number is invalid until the thread is ready to run */
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

