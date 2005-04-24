/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/debug.c,v $
 * $Date: 2005/04/02 08:31:33 $
 * $Revision: 1.12 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: debug.c,v 1.12 2005/04/02 08:31:33 alex Exp $";
#endif

#ifndef DEBUG
#define DEBUG
#endif

#include <pthread.h>
#include <string.h>
#include <sys/debug.h>

#include <unixlib/dev.h>
#include <unixlib/local.h>
#include <unixlib/os.h>
#include <unixlib/sigstate.h>
#include <unixlib/unix.h>

static void
__debugval (const char *s, int i)
{
  const char p[9] = "        ";
  int x;

  __os_print (s);
  if ((x = (strlen (s) & 7)) != 0)
    __os_print (p + x);
  __os_prhex (i);
}

#define NL() __os_nl()
#define VAL(s,i) __debugval(s,i)

void
__debug (const char *s)
{
  unsigned int ui;

  PTHREAD_UNSAFE

  NL ();
  __os_print ("# ");
  if (s)
    __os_print (s);
  NL ();
  VAL ("__image_ro_base: ", (int) __image_ro_base);
  VAL (" __image_rw_lomem: ", (int) __image_rw_lomem);
  VAL (" __image_rw_himem: ", (int) __image_rw_himem);
  NL ();
  VAL ("__u:", (int) __u);
  VAL (" __unixlib_break: ", (int) __unixlib_break);
  VAL (" __unixlib_stack: ", (int) __unixlib_stack);
  /* Make sure the complete __u struct is pointing to valid memory
     otherwise adding __debug() will raise memory exceptions which
     confuses the poor hacker.  */
  if (!__valid_address((const int *)&__u[0], (const int *)&__u[1]))
    {
      __os_print ("__u is pointing to invalid memory\r\n");
      return;
    }
  NL ();
  NL ();
  VAL ("argc:", __u->argc);
  VAL (" argv:", (int) __u->argv);
  NL ();
  if (__u->argv)
    {
      int i;

      for (i = 0; i < __u->argc; i++)
	{
	  VAL ("argv[", (int) (__u->argv + i));
	  VAL (" ", i);
	  VAL (" ]:", (int) __u->argv[i]);
	  __os_print (" ");
	  __os_print (__u->argv[i]);
	  NL ();
	}
    }

  for (ui = 0; ui < __proc->maxfd; ++ui)
    {
      if (getfd (ui)->devicehandle)
	{
	  char fname[_POSIX_PATH_MAX];

	  VAL ("f[", ui);
	  VAL (" ].handle:", (int) getfd (ui)->devicehandle->handle);
	  NL ();
	  /* Do not change this to an malloc'ing version. execve can call
	     this function after it knows no more malloc'ing is done.  */
	  if ((int) getfd (ui)->devicehandle->handle != 0 && getfd (ui)->devicehandle->type == DEV_RISCOS
	      && __fd_to_name ((int) getfd (ui)->devicehandle->handle, fname, sizeof (fname)))
	    {
	      __os_print ("filename:");
	      __os_print (fname);
	      NL ();
	    }
	}
    }
  VAL ("pid:", (int) __proc->pid);
  VAL (" ppid:", (int) __proc->ppid);
  NL ();
  VAL ("sigexit:", (int) __proc->status.signal_exit);
  VAL (" core:", (int) __proc->status.core_dump);
  VAL (" stopped:", (int) __proc->status.stopped);
  NL ();
  VAL ("signal:", (int) __proc->status.signal);
  VAL (" return:", (int) __proc->status.return_code);
  VAL (" dde_prefix:", (int) __u->dde_prefix);
  NL ();
  NL ();
  /* os_get (); */
}
