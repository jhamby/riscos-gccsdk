/* Pthread initialisation.
   Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2010 UnixLib Developers.
   Written by Martin Piper and Alex Waugh */

#include <stdlib.h>
#include <pthread.h>
#include <malloc.h>
#include <swis.h>
#include <string.h>

#include <internal/os.h>
#include <internal/unix.h>

static struct __pthread_thread mainthread;

static const char filter_name[] = "UnixLib pthread";

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

  /* Allocate a small block of RMA to hold some data that remains
   * constant over the life of the program. A pointer to this block
   * will be passed to pthread_call_every in r12. Despite the
   * WimpPoll filters, accessing application space is still crash
   * prone, espacially in the shared library. Passing this data
   * in the RMA allows the application to be validated without having
   * to access its address space therefor making it more robust.
   */
  {
    int regs[10];
  
    regs[0] = 6;
    regs[3] = sizeof (struct __pthread_callevery_block) + sizeof (filter_name);
    if (__os_swi (OS_Module, regs) != NULL)
      __unixlib_fatal ("pthreads initialisation error: out of memory");

    gbl->pthread_callevery_rma = (struct __pthread_callevery_block *)regs[2];
    
#ifdef PIC
    register unsigned int GOT __asm__("v4");

    gbl->pthread_callevery_rma->got = GOT;
#else
    gbl->pthread_callevery_rma->got = 0;
#endif

    gbl->pthread_callevery_rma->sul_upcall_addr = gbl->upcall_handler_addr;
    gbl->pthread_callevery_rma->sul_upcall_r12 = gbl->upcall_handler_r12;
    strcpy (gbl->pthread_callevery_rma->filter_name, filter_name);
  }

  __pthread_thread_list = __pthread_running_thread;
  gbl->pthread_num_running_threads = 1;
}

/* Called once, at program finalisation.  */
void attribute_hidden
__pthread_prog_fini (void)
{
  struct ul_global *gbl = &__ul_global;
#ifdef PTHREAD_DEBUG
  debug_printf ("-- __pthread_prog_fini: Program finalisation\n");
#endif

  /* pthread timers must be stopped */
  if (gbl->pthread_system_running)
    {
      __pthread_stop_ticker ();
      gbl->pthread_system_running = 0;
    }

  /* Free the RMA block that was allocated in __pthread_prog_init */
  int regs[10];
  
  regs[0] = 7;
  regs[2] = (int) gbl->pthread_callevery_rma;
  __os_swi (OS_Module, regs);
}
