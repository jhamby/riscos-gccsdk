/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/init.c,v $
 * $Date: 2003/04/06 10:58:41 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: init.c,v 1.3 2003/04/06 10:58:41 alex Exp $";
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

