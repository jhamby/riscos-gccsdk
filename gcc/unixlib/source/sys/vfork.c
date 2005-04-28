/* Implementation of fork.
   Copyright (c) 2004, 2005 UnixLib Developers.  */

#include <errno.h>
#include <unixlib/unix.h>
#include <pthread.h>
#include <unistd.h>
#include <unixlib/os.h>
#include <swis.h>
#include <unixlib/local.h>
#include <stdlib.h>

static int __saved_pthread_system_running;
static const char *dde_prefix;

/* Do everything that needs doing before the call to sul_fork */
int
__fork_pre (int isfork, void **sul_fork, pid_t *pid)
{
  struct ul_global *gbl = &__ul_global;
  /* Supporting fork when dynamic areas are in use is difficult.
     We would have to take a copy of the main heap DA, and also any mmaped
     regions. Then, when the child exits, we would have to restore them all
     to the same address as from where we copied them from, but the child may
     have free'd some of the original DAs so we would not get the same
     address. Also, if the child ever runs concurrently with the parent, we
     could not support mapping both tasks copies of the DAs to the same
     address.  */
  if (isfork && gbl->__dynamic_num != -1)
    return __set_errno (EINVAL);

  if (gbl->__pthread_system_running)
    {
      __pthread_atfork_callprepare ();
      __pthread_stop_ticker ();
    }

  /* Save __pthread_system_running, as the child process
     will always set it to 0 on exit */
  __saved_pthread_system_running = gbl->__pthread_system_running;

  /* Take a snapshot of DDEUtils_Prefix value. */
  dde_prefix = __get_dde_prefix ();

#if __UNIXLIB_FEATURE_ITIMERS
  /* Stop any interval timers */
  __stop_itimers ();
#endif

  __env_riscos ();

  *sul_fork = (void *)gbl->__proc->sul_fork;
  *pid = gbl->__proc->pid;

  return 0;
}

pid_t
__fork_post (pid_t pid, int isfork)
{
  struct ul_global *gbl = &__ul_global;
  __env_unixlib ();

  gbl->__pthread_system_running = __saved_pthread_system_running;

  if (pid == 0)
    {
      /* We are the child */
      if (isfork)
        {
          int regs[10];
	  struct ul_memory *mem = &__ul_memory;

          /* Read the new application space limit.  */
          regs[0] = 14;
          regs[1] = 0;
          __os_swi (OS_ChangeEnvironment, regs);
          mem->__image_rw_himem = mem->appspace_limit = (void *)regs[1];
        }

      /* There are now two processes sharing the dynamic area.  */
      __atomic_modify (&__dynamic_area_refcount, 1);

      if (gbl->__pthread_system_running)
        __pthread_atfork_callparentchild (0);
    }
  else
    {
      /* We are the parent (including the case when sul_fork failed).  */

      /* Reset the DDEUtils' Prefix variable to the value before the fork,
         in case the child has changed it. */
      if (dde_prefix)
        {
          int regs[10];

          regs[0] = (int) dde_prefix;
          (void) __os_swi (DDEUtils_Prefix, regs);
          free ((void *)dde_prefix);
        }

      /* Disable escape if necessary (in case the child enabled it).  */
      if (__escape_disabled)
        __os_byte (229, 1, 0, NULL);

      if (gbl->__pthread_system_running)
        {
          __pthread_start_ticker ();
          __pthread_atfork_callparentchild (1);
        }

      /* Raise SIGCHLD because the child process has now terminated
         or stopped.  The default action is to ignore this.  */
      if (pid != -1)
        raise (SIGCHLD);
    }

  /* sul_fork can only fail due to lack of memory.  */
  if (pid == -1)
    (void)__set_errno (ENOMEM);

  return pid;
}

