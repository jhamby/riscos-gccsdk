/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/vfork_orig.c,v $
 * $Date: 2000/07/15 14:52:33 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: vfork_orig.c,v 1.1.1.1 2000/07/15 14:52:33 nick Exp $";
#endif

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include <limits.h>
#include <signal.h>

#include <termios.h>
#include <unistd.h>

#include <sys/resource.h>
#include <sys/unix.h>
#include <sys/syslib.h>
#include <sys/param.h>
#include <sys/debug.h>
#include <sys/wait.h>
#include <sys/swis.h>

/* #define DEBUG */
#ifdef DEBUG
#include <stdio.h>
#endif

/* vfork is similar to fork.

   fork makes a complete copy of the calling proces's address space
   and allows both the parent and child to execute independently.
   vfork does not make this copy.

   A child process created with vfork shares its parent's address
   space until it calls exit or one of the exec functions. In the
   meantime, the parent process suspends execution.  */

int *
__vfork (void)
{
  struct proc *child = 0;
  char **argv = 0, *argb = 0;
  size_t *clean_argv = 0;
  char *clean_argb = 0;
  int regs[10];

#ifdef DEBUG
  __debug ("vfork() entered <- parent");
#endif

  if (!(child = malloc (sizeof (struct proc))))
    goto nomem;

  /* Allocate argv array for pointers into argv[] and single string for
     argv[] (i.e., all the arguments) for a copy of the parent argv[].
     We know that the argv[] array is really a single array, and length is
     stored in clean_argv[0].  */
  if (!(argv = malloc (sizeof (char*) * (__u->argc + 1))))
    goto nomem;
  /* Cope if argc == 0  */
  if (!(clean_argv = malloc (sizeof (size_t) * (__u->argc ? __u->argc : 1))))
    goto nomem;
  if (!(argb = malloc (__u->clean_argv[0])))
    goto nomem;
  if (!(clean_argb = malloc (__u->clean_argv[0])))
    goto nomem;

  /* No need to create envp and envb in child proc yet, they will be set at
     execve time from environ.  Initialise them to NULL, so __vexit can
     decide whether to free the memory.  */
  child->envp = NULL;
  child->envb = NULL;

  /* Make the child process structure a copy of its parent.  */
  memcpy (child, __u, sizeof (struct proc));

  /* Copy the parent structure proc for the child process.
     Create a unique ID number for the child. Reading the
     monotonic clock should be okay.  */
  os_swi (OS_ReadMonotonicTime, regs);
  child->ppid = __u->pid;
  child->pid = regs[0];
  /* Simple check to avoid a clash with the parent's pid.  */
  if (child->pid == __u->pid)
    child->pid++;
  child->pproc = __u;
  /* For the time being, the child process will have the same
     priority as the parent process.  */
  /* child->ppri = __u->ppri;	These will have been copied by the memcpy above
     child->gpri = __u->gpri;
     child->upri = __u->upri;  */
  /* Reset the resource usage statistics.  */

  memset (&child->usage, 0, sizeof (struct rusage));
  memset (&child->status, 0, sizeof (struct __process));
  memset (&child->sigstate, 0, sizeof (struct unixlib_sigstate));

  /* A child process always inherits the signals blocked by the
     parent process.  */
  child->sigstate.blocked = __u->sigstate.blocked;
  child->status.tty_type = TTY_CON;

  /* Copy over the command line arguments.  */
  {
    int argc = child->argc;

    memcpy (argb, __u->clean_argb, __u->clean_argv[0]);
    memcpy (clean_argb, __u->clean_argb, __u->clean_argv[0]);
    memcpy (clean_argv, __u->clean_argv, sizeof (int) * (argc ? argc : 1));

    argv[argc] = 0;	/* Write terminating NULL in argv.  */
    if (argc)		/* Don't loop if argc == 0  */
      while (--argc)	/* Don't do argv[0] in this loop.  */
      {
#ifdef DEBUG
	os_prhex (argc);
	os_print ("  ");
	os_prhex (clean_argv[argc]);
	os_print ("\r\n");
#endif
	argv[argc] = argb + clean_argv[argc];
      }
    argv[0] = argb;
  }
  child->argv = argv;
  child->argb = argb;
  child->clean_argv = clean_argv;
  child->clean_argb = clean_argb;

  /* Say that we are a child process to some lucky parent.  */
  child->status.has_parent = 1;

  /* As a process, we now stop executing. The following line will
     make us become the process we've just spawned (ie. the child
     process).  */
  __u = child;

#ifdef DEBUG
  __debug ("vfork() -> child");
#endif

  /* This will now return back to vfork(), with register a1 pointing
     to a jmpbuf which will then be setup by setjmp().  */
  return child->vreg;

nomem:

  if (clean_argb)
    free (clean_argb);
  if (clean_argv)
    free (clean_argv);
  if (argb)
    free (argb);
  if (argv)
    free (argv);
  if (child)
    free (child);

  /* Ensure errno is ENOMEM in case free causes it to be set.  */
  (void) __set_errno (ENOMEM);
  return 0;
}

int *
__vexit (int e)
{
  struct proc *process = __u;
  char **argv = process->argv;
  char *argb = process->argb;
  int i, j;

#ifdef DEBUG
  __debug ("__vexit() entered <- child");
#endif

#if __FEATURE_ITIMERS
  /* Stop any interval timers, just in case.  */
  __stop_itimers ();
#endif

  /* Store the return code, then analyze it, setting various
     bits in the process's status.

     The exit code:
	if bit 7 == 0
	   bits 0..6 = normal exit code
	else
	  {
	     bits 0..5 = signal number that terminated the process.
	     bit 6 == 1 if process core dumped
	  }
  */

  if (e & (1 << 7))
    {
      /* Process terminated with a signal.  */
      process->status.return_code = 0;
      process->status.signal_exit = 1;
      process->status.signal = e & 0x7f;
      process->status.core_dump = (e & (1 << 6)) ? 1 : 0;
    }
  else
    {
      process->status.return_code = e & 0x7f;
      process->status.core_dump = 0;
      process->status.signal_exit = 0;
    }

  /* Close the file descriptors we used.  */
  for (i = 0; i < MAXFD; i++)
    if (process->fd[i].__magic == _FDMAGIC)
      close (i);

#ifdef DEBUG
  __debug ("__vexit() <- child (close())");
  os_print ("__vexit() e = ");
  os_prhex (e);
  os_print ("\r\n");
  os_print ("return code = ");
  os_prhex (process->status.return_code);
  os_print ("\r\n");
  os_print ("signal exit = ");
  os_prhex (process->status.signal_exit);
  os_print ("\r\n");
  os_print ("signal = ");
  os_prhex (process->status.signal);
  os_print ("\r\n");
  os_print ("core dumped = ");
  os_prhex (process->status.core_dump);
  os_print ("\r\n");
#endif

  /* Make the current process structure now point to the
     child's parent process structure. i.e. our process
     effectively terminates and the parent continues execution.  */
  __u = process->pproc;
#if 0
  i = __find_free_child ();
  if (i != -1)
    {
      /* Copy over child structure data we will need to remember
	 for the wait functions.  */
#endif
      __u->child[0].pid = process->pid;
      __u->child[0].uid = process->uid;
      __u->child[0].gid = process->gid;
      __u->child[0].ppri = process->ppri;
      __u->child[0].upri = process->upri;
      __u->child[0].gpri = process->gpri;
      __u->child[0].usage = process->usage;
      __u->child[0].status = process->status;

      for (j = 0; j < __JMP_BUF_SIZE; j++)
	__u->child[0].vreg[j] = process->vreg[j];
      __u->child[0].vreg[13] = process->pid;

#if 0
    }
#endif

  free (argb);
  free (argv);
  free (process->envp);
  free (process->envb);
  free (process);

#ifdef DEBUG
  __debug ("__vexit() -> parent");
#endif
  /* Raise SIGCHLD because the child process has now terminated
     or stopped.  The default action is to ignore this.  */
  raise (SIGCHLD);

  return __u->child[0].vreg;
}
