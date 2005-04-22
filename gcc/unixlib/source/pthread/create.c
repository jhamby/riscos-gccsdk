/* Thread creation.
   Copyright (c) 2004, 2005 UnixLib Developers.
   Written by Martin Piper and Alex Waugh.  */


#include <stdlib.h>
#include <errno.h>
#include <unixlib/os.h>
#include <unixlib/unix.h>
#include <pthread.h>

/* __pthread_create is the first thing called when a new thread
   is switched to.  */
static void
__pthread_create (pthread_t thread)
{
  void *ret = NULL;

#ifdef PTHREAD_DEBUG
  __os_print ("-- __pthread_create: About to start thread ");
  __os_prhex ((int) thread);
  __os_nl ();
#endif

  /* Go into the thread properly.  */
  ret = (*thread->start_routine) (thread->arg);

#ifdef PTHREAD_DEBUG
  __os_print ("-- __pthread_create: Thread ");
  __os_prhex ((int) thread);
  __os_print (" has returned\r\n");
#endif

  /* A thread returning from its main function is an implicit call
     to pthread_exit().  */
  pthread_exit (ret);  /* This never returns */
}

int
pthread_create (pthread_t *threadin, const pthread_attr_t *attr,
		void * (*start_routine) (void *), void *arg)
{
  pthread_t thread;

  if (threadin == NULL || start_routine == NULL)
    return EINVAL;

  /* If the pthread subsystem wasn't running before, set the flag to
     indicate that it should be now.  */
  if (! __pthread_system_running)
    __pthread_system_running = 1;

  __pthread_disable_ints ();

#ifdef PTHREAD_DEBUG
  __os_print ("-- pthread_create: Starting new thread\r\n");
#endif

  /* Allocate memory for a new thread, without adding the thread to
     any lists.  */
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
        __proc->sul_free (__proc->pid, thread->saved_context);

      __proc->sul_free (__proc->pid, thread);

#ifdef PTHREAD_DEBUG
      __os_print ("-- pthread_create: Cannot start any more threads at the moment (no memory)\r\n");
#endif
      __pthread_enable_ints () ;
      return EAGAIN;
    }


  if (attr != NULL)
    thread->detachstate = attr->detachstate;

  __pthread_init_save_area (thread->saved_context);

  /* Initialise stack frame registers, ready for the first context switch.  */

  /* Set stack-pointer register.  */
  thread->saved_context->r[13] = (int)((char *)thread->stack
				       + PTHREAD_STACK_MIN);

  /* Set stack-limit register.  */
  thread->saved_context->r[10] = (int)((char *)thread->stack
				       + 512
				       + sizeof (struct __stack_chunk));

  /* Set the frame pointer to zero.  During stack backtraces, a frame
     pointer of zero means that this is the top-level stack frame.  */
  thread->saved_context->r[11] = 0;

  thread->saved_context->r[0] = (int)thread; /* set R0 */

  /* Set the program counter to point to the __pthread_create routine.
     The first context switch to the new thread will then automatically
     call the real startup function of the thread.  */
  thread->saved_context->r[15] = (int)__pthread_create;

  /* Set the program status register to USR32 mode and interrupts enabled.  */
  thread->saved_context->spsr = 0x00000010;

  thread->start_routine = start_routine;
  thread->arg = arg;

  thread->magic = PTHREAD_MAGIC;

  /* Thread structure is now initialised, insert it at the start of the
     process queue.  */
  thread->next = __pthread_thread_list;
  __pthread_thread_list = thread;

  *threadin = thread;

  __pthread_num_running_threads++;
  __pthread_start_ticker ();
  __pthread_enable_ints ();

  return 0;
}
