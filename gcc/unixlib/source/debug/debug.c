/* Internal UnixLib structure debugging.
   Copyright (c) 2002, 2003, 2004, 2005 UnixLib Devlopers.  */

#include <pthread.h>
#include <string.h>
#include <sys/debug.h>

#include <unixlib/dev.h>
#include <unixlib/local.h>
#include <unixlib/os.h>
#include <unixlib/sigstate.h>
#include <unixlib/unix.h>

void
__debug (const char *s)
{
  unsigned int ui;

  PTHREAD_UNSAFE
  debug_printf ("\n# %s\n", (s) ? s : "");
  debug_printf ("__image_ro_base: %08x, __image_rw_lomem: %08x,"
		" __image_rw_himem: %08x\n",
		__image_ro_base, __image_rw_lomem, __image_rw_himem);

  debug_printf ("__u: %08x, __unixlib_break: %08x, __unixlib_stack: %08x\n",
		__u, __unixlib_break, __unixlib_stack);

  /* Make sure the complete __u struct is pointing to valid memory
     otherwise adding __debug() will raise memory exceptions which
     confuses the poor hacker.  */
  if (!__valid_address((const int *)&__u[0], (const int *)&__u[1]))
    {
      debug_printf ("__u is pointing to invalid memory\n");
      return;
    }
  debug_printf ("\nargc: %d, argv:\n", __u->argc);
  if (__u->argv)
    {
      int i;

      for (i = 0; i < __u->argc; i++)
	{
	  debug_printf ("argv[%d] (%08x): %s\n",
			i, __u->argv + i, __u->argv[i]);
	}
    }

  for (ui = 0; ui < __proc->maxfd; ++ui)
    {
      if (getfd (ui)->devicehandle)
	{
	  char fname[_POSIX_PATH_MAX];

	  debug_printf ("f[%d].handle: %d\n",
			ui, getfd (ui)->devicehandle->handle);

	  /* Do not change this to an malloc'ing version. execve can call
	     this function after it knows no more malloc'ing is done.  */
	  if ((int) getfd (ui)->devicehandle->handle != 0
	      && getfd (ui)->devicehandle->type == DEV_RISCOS
	      && __fd_to_name ((int) getfd (ui)->devicehandle->handle,
			       fname, sizeof (fname)))
	    {
	      debug_printf ("filename: %s\n", fname);
	    }
	}
    }
  debug_printf ("pid: %d, ppid: %d\n", __proc->pid, __proc->ppid);
  debug_printf ("sigexit: %d, core: %d, stopped: %d\n",
		__proc->status.signal_exit,
		__proc->status.core_dump,
		__proc->status.stopped);
  debug_printf ("signal: %d, return: %d, dde_prefix=%s\n",
		__proc->status.signal,
		__proc->status.return_code,
		__u->dde_prefix);
}
