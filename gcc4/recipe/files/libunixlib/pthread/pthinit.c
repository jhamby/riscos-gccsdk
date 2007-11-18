/* Pthread initialisation.
   Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007 UnixLib Developers.
   Written by Martin Piper and Alex Waugh */

#include <unixlib/os.h>
#include <unixlib/unix.h>
#include <stdlib.h>
#include <pthread.h>
#include <malloc.h>

static struct __pthread_thread mainthread;

/* Called once, at program initialisation.  */
void
__pthread_prog_init (void)
{
  struct ul_global *gbl = &__ul_global;
#ifdef PTHREAD_DEBUG
  debug_printf ("-- __pthread_prog_init: Program initialisation\n");
#endif

  /* Create a node for the main program.  Calling this function with a
     valid argument means that we must setup 'saved_context' ourselves.  */
  __pthread_running_thread = __pthread_new_node (&mainthread);
  if (__pthread_running_thread == NULL)
    __unixlib_fatal ("pthreads initialisation error: out of memory");

  mainthread.saved_context =
    malloc_unlocked (gbl->malloc_state,
		     sizeof (struct __pthread_saved_context));
  if (mainthread.saved_context == NULL)
    __unixlib_fatal ("pthreads initialisation error: out of memory");
  mainthread.magic = PTHREAD_MAGIC;

  __pthread_thread_list = __pthread_running_thread;
  gbl->pthread_num_running_threads = 1;
}
