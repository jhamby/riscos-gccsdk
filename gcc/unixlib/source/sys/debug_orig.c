/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/debug_orig.c,v $
 * $Date: 2000/07/15 14:52:32 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: debug_orig.c,v 1.1.1.1 2000/07/15 14:52:32 nick Exp $";
#endif

#ifndef DEBUG
#define DEBUG
#endif

/* __debug(s) dumps UNIX status with title "s" */

#include <string.h>

#include <sys/dev.h>
#include <sys/os.h>
#include <sys/unix.h>
#include <sys/syslib.h>
#include <sys/debug.h>
#include <unixlib/local.h>

static void
__debugval (char *s, int i)
{
  char *p = "        ";
  int x;

  os_print (s);
  x = (strlen (s) & 7);
  x = x ? x : 8;
  os_print (p + x);
  os_prhex (i);
}

static void
__debugnl (void)
{
  os_print ("\r\n");
}

#define NL() __debugnl()
#define VAL(s,i) __debugval(s,i)

void
__debug (const char *s)
{
  struct __unixlib_fd *f;
  int i;

  NL ();
  os_print ("# ");
  if (s)
    os_print (s);
  NL ();
  VAL ("__base: ", (int) __base);
  NL ();
  VAL ("__lomem: ", (int) __lomem);
  VAL (" __himem: ", (int) __himem);
  NL ();
  VAL ("__break: ", (int) __break);
  VAL (" __stack: ", (int) __stack);
  NL ();
  VAL ("__u:", (int) __u);
  NL ();
  VAL ("argc:", __u->argc);
  VAL (" argv:", (int) __u->argv);
  NL ();
  for (i = 0; i < __u->argc; i++)
    {
      VAL ("argv[", (int) (__u->argv + i));
      VAL (" ", i);
      VAL (" ]:", (int) __u->argv[i]);
      os_print (" ");
      os_print (__u->argv[i]);
      NL ();
    }
  VAL ("argb:", (int) __u->argb);
  os_print (" ");
  os_print (__u->argb);
  NL ();
  VAL (" clean_argv:", (int) __u->clean_argv);
  NL ();
  for (i = 0; i < __u->argc; i++)
    {
      VAL ("clean_argv[", (int) (__u->clean_argv + i));
      VAL (" ", i);
      VAL (" ]:", (int) __u->clean_argv[i]);
      NL ();
    }
  VAL ("clean_argb:", (int) __u->clean_argb);
  os_print (" ");
  os_print (__u->clean_argb);
  NL ();
  f = __u->fd;
  if (f)
    for (i = 0; i < MAXFD; i++)
      if (f[i].__magic == _FDMAGIC)
	{
	  char fname[_POSIX_PATH_MAX];

	  VAL ("f[", (int) (f + i));
	  VAL (" ", i);
	  VAL (" ].handle:", (int) f[i].handle);
	  NL ();
	  /* Do not change this to an malloc'ing version. execve can call
	     this function after it knows no more malloc'ing is done.  */
	  if ((int) f[i].handle != 0 && f[i].device == DEV_RISCOS
	      && __fd_to_name ((int) f[i].handle, fname, sizeof (fname)))
	    {
	      os_print ("filename:");
	      os_print (fname);
	      NL ();
	    }
	}
  VAL ("pid:", (int) __u->pid);
  VAL (" ppid:", (int) __u->ppid);
  NL ();
  VAL ("pproc:", (int) __u->pproc);
  NL ();
  VAL ("signal_exit:", (int) __u->status.signal_exit);
  NL ();
  VAL ("core_dump:", (int) __u->status.core_dump);
  NL ();
  VAL ("stopped:", (int) __u->status.stopped);
  NL ();
  VAL ("signal:", (int) __u->status.signal);
  NL ();
  VAL ("return_code:", (int) __u->status.return_code);
  NL ();
  VAL (" vreg:", (int) __u->vreg);
  NL ();
  NL ();
  /* os_get (); */
}
