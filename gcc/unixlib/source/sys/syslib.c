/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/sys/c/syslib,v $
 * $Date: 2000/01/12 16:52:26 $
 * $Revision: 1.26 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: syslib,v 1.26 2000/01/12 16:52:26 admin Exp $";
#endif

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/syslib.h>
#include <sys/mman.h>
#include <sys/os.h>
#include <sys/unix.h>
#include <sys/swis.h>
#include <sys/wait.h>
#include <unixlib/sigstate.h>
#include <unixlib/local.h>

struct env __c_environment[1] =
{
  {
    {
      {0, 0, 0},		/* MemoryLimit */
      {__h_sigill, 0, 0},	/* Undefined Inst. */
      {__h_sigsegv0, 0, 0},	/* Prefetch Abort */
      {__h_sigsegv1, 0, 0},	/* Data Abort */
      {__h_sigbus, 0, 0},	/* Address Exception */
      {0, 0, 0},		/* Other Exception */
      {__h_error, 0, 0},	/* Error */
      {__h_cback, 0, (void *) __cbreg},		/* CallBack */
      {0, 0, 0},		/* BreakPoint */
      {__h_sigint, 0, 0},	/* Escape */
      {__h_event, 0, 0},	/* Event */
      {__h_exit, 0, 0},		/* Exit */
      {0 /* __h_sigsys */ , 0, 0},	/* Unknown SWI */
      {0, 0, 0},		/* Exception registers.  */
      {0, 0, 0},		/* Application space.  */
      {0, 0, 0},		/* Currently active object.  */
      {__h_upcall, 0, 0}	/* UpCall.  */
    }
  }
};

struct env __calling_environment[1];

struct proc *__u;

/* This is the only file that need ever reference main()
   so to prevent possible compiler errors from sources that
   might include <sys/syslib.h> we will have the main()
   declaration here.  */
extern int main (int argc, char *argv[], char **environ);

/* The order of initialisation in this function is relied on by
   many functions.  Be careful before you change anything.  */
void
_main (void)
{
  struct env *environment;
  int i;

#ifdef DEBUG
  os_print ("Installing environment handlers\r\n");
#endif
  /* Install environment handlers for the C signal handler.  */
  environment = __calling_environment;
  for (i = 0; i < __ENVIRONMENT_HANDLERS; i++)
    __read_environment_handler (i, environment->handlers + i);

  __h_errbuf = (char *) environment->handlers[6].buffer;

  environment = __c_environment;
  for (i = 0; i < __ENVIRONMENT_HANDLERS; i++)
    __write_environment_handler (i, environment->handlers + i);

  /* Initialise ctype tables to the C locale.  */
  __build_ctype_tables (-2);

#ifdef DEBUG
  os_print ("Initialising unix. Creating process structure.\n\r");
#endif
  /* Initialise UNIX.  */
  __unixinit ();

#ifdef DEBUG
  os_print ("Initialising stdio.\n\r");
#endif
  /* Initialise stdin, stdout & stderr.  */
  if (___stdioinit)
    __funcall ((*___stdioinit), ());

#ifdef DEBUG
  os_print ("Going main...\r\n");
#endif
  /* Enter the user's program. For compatibility with Unix systems,
     pass the 'environ' variable as a third argument.  */
  exit (main (__u->argc, __u->argv, environ));

/* This is never reached. */
}

void (*__atexit_function_array[__MAX_ATEXIT_FUNCTION_COUNT]) (void);
int __atexit_function_count = 0;

void
exit (int status)
{
  int i;

  /* Execute atexit functions in the reverse order of their
     registration.  */
  i = __atexit_function_count;
  while (i)
    {
      i--;
      __funcall ((*__atexit_function_array[i]), ());
    }

  /* Close all open streams, write out buffered output data and
     delete tmpfile() temporary files.  */
  if (___stdioexit)
    __funcall ((*___stdioexit), ());

  /* Only return codes between 0 and 127 are valid.  128 to 255 are
     reserved for the run-time library internals.  */
  status = status & 0x7f;

  _exit (__W_EXITCODE (status, 0));
}

void __restore_calling_environment_handlers (void)
{
  int i;
  struct env *environment;

  environment = __calling_environment;
  for (i = 0; i < __ENVIRONMENT_HANDLERS; i++)
    __write_environment_handler (i, environment->handlers + i);
}

void
__reset (void)
{
  /* Shutdown UNIX (close all fds etc.).  */
  if (__u)
    __unixexit ();

  __restore_calling_environment_handlers ();
}

/* Final process termination. 'return_code' is a 16-bit
   encoded quantity as defined by <sys/wait.h>.  */
void
_exit (int return_code)
{
  int status;

#if __FEATURE_ITIMERS
  /* Interval timers must be stopped.  */
  if (__u)
    __stop_itimers ();
#endif

  /* Convert the 16-bit return code into an 8-bit equivalent
     for compatibility with RISC OS.  See sys.c.vfork for
     further information.  */
  if (WIFSIGNALED (return_code))
    {
      status = WTERMSIG (return_code);
      status |= (1 << 7);
      if (WCOREDUMP (return_code))
	status |= (1 << 6);
    }
  else
    {
      status = WEXITSTATUS (return_code);
    }

  /* If we aren't a child process then we can just exit the system.  */
  if (!__u || !__u->status.has_parent || !___vret)
    {
      __reset ();

#ifdef DEBUG
      os_print ("_exit(): Setting return code = ");
      os_prhex (return_code);
      os_print ("\r\n");
#endif

      /* OS_Exit with return value 'r'.  This function never returns.  */
      __exit (status);
    }

  /* Due to the change to clock(), this is currently complete crap.  */

#ifdef DEBUG
  os_print ("_exit(): calling ___vret with return code = ");
  os_prhex (return_code);
  os_print ("\r\n");
#endif

  if (___vret)
    __funcall ((*___vret), (status));
}
