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

#include <unixlib/os.h>
#include <unixlib/unix.h>
#include <stdlib.h>
#include <pthread.h>

struct __pthread_thread mainthread;

/* Called once, at program initialisation */
void
__pthread_prog_init (void)
{
#ifdef PTHREAD_DEBUG
  __os_print ("-- __pthread_prog_init: Program initialisation\r\n");
#endif

  /* Create a node for the main program */
  __pthread_running_thread = __pthread_new_node (&mainthread);

  __pthread_thread_list = __pthread_running_thread;

  __pthread_running_threads = 1;

  __pthread_running_thread->magic = PTHREAD_MAGIC;
}




/* Called once, the first time a pthread call is made */
void
__pthread_init (void)
{
  void *context_stack;

  if (__pthread_system_running)
    return;

#ifdef PTHREAD_DEBUG
  __os_print ("-- __pthread_init: Initialising\r\n");
#endif

  if (__pthread_invalid (__pthread_running_thread))
    __pthread_fatal_error ("-- __pthread_init: Main thread not initialised\r\n");

  __pthread_running_thread->saved_context = malloc (sizeof (struct __pthread_saved_context));
  if (__pthread_running_thread->saved_context == NULL)
    __pthread_fatal_error ("-- __pthread_init: Unable to start threads (Out of memory)\r\n");

  /* Set up a stack for the context switcher */
  context_stack = __pthread_new_stack ();
  if (context_stack == NULL)
    __pthread_fatal_error ("-- __pthread_init: Unable to start threads (Out of memory)\r\n");

  __pthread_context_stack_sp = (char *)context_stack + PTHREAD_STACK_MIN;
  __pthread_context_stack_sl = (char *)context_stack + sizeof (struct __stack_chunk) + 512;

#ifdef PTHREAD_DEBUG
  __os_print ("-- __pthread_init: Starting the interupts\r\n");
#endif
  __pthread_system_running = 1;
  __pthread_start_ticker ();
}

