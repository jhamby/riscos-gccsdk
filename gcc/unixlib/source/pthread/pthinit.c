/* Pthread initialisation.
   Copyright (c) 2004, 2005 UnixLib Developers.  */

/* Written by Martin Piper and Alex Waugh */

#include <unixlib/os.h>
#include <unixlib/unix.h>
#include <stdlib.h>
#include <pthread.h>

static struct __pthread_thread mainthread;

/* Called once, at program initialisation */
void
__pthread_prog_init (void)
{
#ifdef PTHREAD_DEBUG
  __os_print ("-- __pthread_prog_init: Program initialisation\r\n");
#endif

  /* Create a node for the main program */
  __pthread_running_thread = __pthread_new_node (&mainthread);
  if (__pthread_running_thread == NULL)
    __unixlib_fatal ("pthreads initialisation error.");

  __pthread_thread_list = __pthread_running_thread;

  __pthread_num_running_threads = 1;

  __pthread_running_thread->magic = PTHREAD_MAGIC;
}


/* Called once, the first time a pthread call is made */
void
__pthread_init (void)
{
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

#ifdef PTHREAD_DEBUG
  __os_print ("-- __pthread_init: Starting the interupts\r\n");
#endif
  __pthread_system_running = 1;
}
