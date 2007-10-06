/* Internal UnixLib structure debugging.
   Copyright (c) 2002, 2003, 2004, 2005, 2007 UnixLib Devlopers.  */

#include <pthread.h>
#include <string.h>
#define DEBUG
#include <sys/debug.h>

#include <unixlib/dev.h>
#include <unixlib/local.h>
#include <unixlib/os.h>
#include <unixlib/sigstate.h>
#include <unixlib/unix.h>

void
__debug (const char *s)
{
  const struct ul_memory *mem = &__ul_memory;
  const struct ul_global *gbl = &__ul_global;
  const struct __sul_process *sulproc = gbl->sulproc;

  PTHREAD_UNSAFE

  debug_printf ("\n# %s\n", (s) ? s : "");
  debug_printf ("robase: %08x, rwbase: %08x, rwlomem: %08x\n"
		"appspace_himem: %08x, appspace_limit: %08x\n"
		"rwbreak: %08x, stack_limit: %08x, stack: %08x\n"
		"dalomem: %08x, dabreak: %08x, dalimit: %08x\n",
		mem->robase, mem->rwbase, mem->rwlomem,
		mem->appspace_himem, mem->appspace_limit,
		mem->rwbreak, mem->stack_limit, mem->stack,
		mem->dalomem, mem->dabreak, mem->dalimit);

  /* Make sure the complete __u struct is pointing to valid memory
     otherwise adding __debug() will raise memory exceptions which
     confuses the poor hacker.  */
  debug_printf ("__u: %08x\n", __u);
  if (!__valid_address((const void *)&__u[0], (const void *)&__u[1]))
    debug_printf ("__u is pointing to invalid memory\n");
  else
    {
      debug_printf ("\nargc: %d, argv: %08x\n", __u->argc, __u->argv);
      if (__u->argv)
        {
          int i;

          for (i = 0; i < __u->argc; i++)
	    debug_printf ("argv[%d] (%08x): %s\n",
			  i, __u->argv + i, __u->argv[i]);
        }
      debug_printf ("dde_prefix: %s\n", __u->dde_prefix);
    }

  if (!__valid_address((const void *)&sulproc[0], (const void *)&sulproc[1]))
    debug_printf ("__ul_global.sulproc is pointing to invalid memory: %08x\n",
		  sulproc);
  else
    {
      unsigned int ui;

      for (ui = 0; ui < sulproc->maxfd; ++ui)
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
	        debug_printf ("filename: %s\n", fname);
	    }
        }
      debug_printf ("pid: %d, ppid: %d\n", sulproc->pid, sulproc->ppid);
      debug_printf ("sigexit: %d, core: %d, stopped: %d\n",
		    sulproc->status.signal_exit,
		    sulproc->status.core_dump,
		    sulproc->status.stopped);
      debug_printf ("signal: %d, return: %d\n",
		    sulproc->status.signal,
		    sulproc->status.return_code);
    }
}
