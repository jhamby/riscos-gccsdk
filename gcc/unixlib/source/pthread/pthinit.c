/* Pthread initialisation.
   Copyright (c) 2004, 2005 UnixLib Developers.  */

/* Written by Martin Piper and Alex Waugh */

#include <unixlib/os.h>
#include <unixlib/unix.h>
#include <stdlib.h>
#include <pthread.h>

static struct __pthread_thread mainthread;

/* Called once, at program initialisation.  */
void
__pthread_prog_init (void)
{
#ifdef PTHREAD_DEBUG
  __os_print ("-- __pthread_prog_init: Program initialisation\r\n");
#endif

  /* Create a node for the main program.  Calling this function with a
     valid argument means that we must setup 'saved_context' ourselves.  */
  __pthread_running_thread = __pthread_new_node (&mainthread);
  if (__pthread_running_thread == NULL)
    __unixlib_fatal ("pthreads initialisation error: out of memory, increase RMA");

  mainthread.saved_context =
    __proc->sul_malloc (__proc->pid, sizeof (struct __pthread_saved_context));
  if (mainthread.saved_context == NULL)
    __unixlib_fatal ("pthreads initialisation error: out of memory, increase RMA");

  __pthread_thread_list = __pthread_running_thread;
  __pthread_num_running_threads = 1;
  mainthread.magic = PTHREAD_MAGIC;
}
