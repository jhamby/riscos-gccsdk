/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/vfork.c,v $
 * $Date: 2004/09/07 14:05:11 $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: vfork.c,v 1.8 2004/09/07 14:05:11 joty Exp $";
#endif

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include <limits.h>
#include <signal.h>

#include <time.h>

#include <termios.h>
#include <unistd.h>

#include <sys/resource.h>
#include <unixlib/unix.h>
#include <sys/param.h>
#include <sys/debug.h>
#include <sys/wait.h>
#include <unixlib/os.h>
#include <swis.h>
#include <unixlib/features.h>
#include <unixlib/local.h>

/* #define DEBUG 1 */

#include <sys/debug.h>

static void free_process (struct proc *p)
{
  int x;

  if (p == NULL)
    return;

  /* Free any parts of the process structure that we've just created.  */
  if (p->argv)
    {
      for (x = 0; x < p->argc; x++)
	if (p->argv[x])
	  free (p->argv[x]);
      free (p->argv);
    }

  if (p->envp)
    {
      for (x = 0; x < p->envc; x++)
	if (p->envp[x])
	  free (p->envp[x]);
      free (p->envp);
    }

  if (p->dde_prefix)
    free ((void *)p->dde_prefix);

  free (p);
}

/* vfork is similar to fork.

   fork makes a complete copy of the calling process's address space
   and allows both the parent and child to execute independently.
   vfork does not make this copy.

   A child process created with vfork shares its parent's address
   space until it calls exit or one of the exec functions. In the
   meantime, the parent process suspends execution.  */
int *
__vfork (void)
{
  struct proc *child;
  int x;

#ifdef DEBUG
  __debug ("vfork: parent process structure");
#endif

  /* Allocate memory for the new child process structure.  */
  child = malloc (sizeof (struct proc));
  if (child == NULL)  /* No memory.  */
    goto nomem;

  /* Initialise structure.  */
  memset (child, 0, sizeof (struct proc));

  /* Create a process ID.  It is cheaper to call clock() than __os_swi.  */
  child->ppid = __u->pid;  /* Get parent process's ID.  */
  child->pid = (pid_t) clock ();  /* Child process ID.  */

  /* Make sure child's PID is not the same as the parents.  */
  if (child->pid == __u->pid)
    child->pid ++;

  /* Keep a pointer to the parent process structure.  */
  child->pproc = __u;

  /* Give the child process the same priorities as the parent process.  */
  child->ppri = __u->ppri;
  child->gpri = __u->gpri;
  child->upri = __u->upri;

  /* A child process always inherits the signals blocked by the
     parent process.  */
  child->sigstate.blocked = __u->sigstate.blocked;
  child->status.tty_type = TTY_CON;

  /* Make a copy of the command line arguments.  */
  if (__u->argc)
    {
      child->argv = (char **) malloc ((__u->argc + 1) * sizeof (char *));
      if (child->argv == NULL)
        goto nomem;
      /* Copy entries from the environment and argument vectors.  */
      for (x = 0; x < __u->argc; x++)
        child->argv[x] = strdup (__u->argv[x]);
      child->argv[x] = NULL;
      child->argc = __u->argc;
    }

  /* Copy environment vector.  */
  if (__u->envc)
    {
      child->envp = (char **) malloc ((__u->envc + 1) * sizeof (char *));
      if (child->envp == NULL)
        goto nomem;
      for (x = 0; x < __u->envc; x++)
        child->envp[x] = strdup (__u->envp[x]);
      child->envp[x] = NULL;
      child->envc = __u->envc;
    }

  /* Copy resource usage of the parent process.  */
  child->usage = __u->usage;

  /* I am a child process and I have a parent.  How nice.  */
  child->status.has_parent = 1;

  /* Copy file descriptors.  */
  for (x = 0; x < MAXFD; x++)
    child->fd[x] = __u->fd[x];

  /* Copy process resource limits.  */
  for (x = 0; x < RLIMIT_NLIMITS; x++)
    child->limit[x] = __u->limit[x];

  /* Copy tty.  */
  child->tty = __u->tty;

  /* Take a snapshot of DDEUtils_Prefix value.  */
  child->dde_prefix = __get_dde_prefix ();

  child->__magic = _PROCMAGIC;

  /* As a process, we now stop executing. The following line will
     make us become the process we've just spawned (ie. the child
     process).  */
  __u = child;

#ifdef DEBUG
  __debug ("vfork: new child process structure");
#endif

  /* This will now return back to vfork(), with register a1 pointing
     to a jmpbuf which will then be setup by setjmp().  */
  return child->vreg;

nomem:
  free_process (child);
  /* Ensure errno is ENOMEM in case free causes it to be set.  */
  (void) __set_errno (ENOMEM);
  return NULL;
}

int *
__vexit (int e)
{
  struct proc *p = __u;
  int x;
  int regs[10];

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
      p->status.return_code = 0;
      p->status.signal_exit = 1;
      p->status.signal = e & 0x7f;
      p->status.core_dump = (e & (1 << 6)) ? 1 : 0;
    }
  else
    {
      p->status.return_code = e & 0x7f;
      p->status.core_dump = 0;
      p->status.signal_exit = 0;
    }

#ifdef DEBUG
  __debug ("__vexit: child process structure");
  __os_print ("__vexit() e = "); __os_prdec (e); __os_print ("\r\n");
  __os_print ("return code:"); __os_prdec (p->status.return_code); __os_print ("\r\n");
  __os_print ("signal exit:"); __os_prdec (p->status.signal_exit); __os_print ("\r\n");
  __os_print ("signal:"); __os_prdec (p->status.signal); __os_print ("\r\n");
  __os_print ("core dumped:"); __os_prdec (p->status.core_dump); __os_print ("\r\n");
#endif

  /* Close the file descriptors we used.  */
  for (x = 0; x < MAXFD; x++)
    if (p->fd[x].__magic == _FDMAGIC)
      close (x);

#ifdef DEBUG
  __os_print ("__vexit: child died, now becoming parent process\r\n");
#endif
  /* Become the parent process.  */
  __u = p->pproc;
  /* Copy some parts of the old child process structure.  */
  __u->child[0].pid = p->pid;
  __u->child[0].uid = p->uid;
  __u->child[0].gid = p->gid;
  __u->child[0].ppri = p->ppri;
  __u->child[0].upri = p->upri;
  __u->child[0].gpri = p->gpri;
  __u->child[0].usage = p->usage;
  __u->child[0].status = p->status;

  memcpy(__u->child[0].vreg, p->vreg, __JMP_BUF_SIZE * sizeof(p->vreg[0]));
  /* Copy the pid of the old child process into the jmp_buf */
  __u->child[0].vreg[15] = p->pid;

  /* Restore DDEUtils_Prefix value.  */
  regs[0] = (int)p->dde_prefix;
  (void) __os_swi (DDEUtils_Prefix, regs);

  /* Free memory allocated to the child process.  */
  free_process (p);

#ifdef DEBUG
  __debug ("__vexit: parent process structure");
#endif
  /* Raise SIGCHLD because the child process has now terminated
     or stopped.  The default action is to ignore this.  */
#ifdef DEBUG
  __os_print ("__vexit: raising SIGCHLD\r\n");
#endif
  raise (SIGCHLD);
#ifdef DEBUG
  __os_print ("__vexit: raising SIGCHLD completed\r\n");
#endif
  return __u->child[0].vreg;
}
