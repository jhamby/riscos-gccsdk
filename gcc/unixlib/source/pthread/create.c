/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/create.c,v $
 * $Date: 2002/12/15 13:16:55 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: create.c,v 1.1 2002/12/15 13:16:55 admin Exp $";
#endif

/* Thread creation */

/* Written by Martin Piper and Alex Waugh */

#include <stdlib.h>
#include <errno.h>
#include <unixlib/os.h>
#include <unixlib/unix.h>
#include <pthread.h>


/* __pthread_create is the first thing called when a new thread is switched to */
static void
__pthread_create (pthread_t thread)
{
  void *ret = NULL;

#ifdef PTHREAD_DEBUG
  __os_print ("-- __pthread_create: About to start thread ");
  __os_prhex ((int) thread);
  __os_print ("\r\n");
#endif

  /* Go into the thread properly */
  ret = (*thread->start_routine) (thread->arg);

#ifdef PTHREAD_DEBUG
  __os_print ("-- __pthread_create: Thread ");
  __os_prhex ((int) thread);
  __os_print (" has returned\r\n");
#endif

  /* A thread returning from its main function is an implicit call to pthread_exit() */
  pthread_exit (ret);  /* This never returns */
}

int
pthread_create (pthread_t *threadin, const pthread_attr_t *attr, void * (*start_routine) (void *), void *arg)
{
  pthread_t thread;

  if (!__pthread_system_running) __pthread_init ();

  if (threadin == NULL)
    return EINVAL;
  if (start_routine == NULL)
    return EINVAL;

  __pthread_disable_ints ();

#ifdef PTHREAD_DEBUG
  __os_print ("-- pthread_create: Starting new thread\r\n");
#endif

  /* Allocate without actually adding the node into any lists */
  thread = __pthread_new_node (NULL);

  if (thread == NULL)
    {
      __pthread_enable_ints ();
      return EAGAIN;
    }

  thread->stack = __pthread_new_stack ();

  if (thread->saved_context == NULL || thread->stack == NULL)
    {
      if (thread->stack != NULL)
        __free_stack_chain (thread->stack);

      if (thread->saved_context != NULL)
        free (thread->saved_context);

      free (thread);

#ifdef PTHREAD_DEBUG
      __os_print ("-- pthread_create: Cannot start any more threads at the moment (no memory)\r\n");
#endif
      __pthread_enable_ints () ;
      return EAGAIN;
    }


  if (attr != NULL)
    thread->detachstate = attr->detachstate;


  __pthread_init_save_area (thread->saved_context);

  thread->saved_context->r[13] = (int)((char *)thread->stack + PTHREAD_STACK_MIN); /* set sp */
  thread->saved_context->r[10] = (int)((char *)thread->stack + 512 + sizeof (struct __stack_chunk)); /* set sl */
  thread->saved_context->r[11] = 0; /* set fp */
  thread->saved_context->r[0] = (int)thread; /* set R0 */
  /* Because this is a thread start call we return instantly.
     __pthread_create handles the real starting up of this new
     thread when the context is reached */
  thread->saved_context->r[15] = (int)__pthread_create; /* set pc */

  thread->start_routine = start_routine;
  thread->arg = arg;

  /* Place into start of thread queue */
  thread->next = __pthread_thread_list;
  __pthread_thread_list = thread;

  thread->magic = PTHREAD_MAGIC;

  *threadin = thread;

  __pthread_running_threads++;

  __pthread_start_ticker ();

  __pthread_enable_ints ();


  return 0;
}


