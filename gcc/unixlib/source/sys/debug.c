/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/debug.c,v $
 * $Date: 2004/06/12 08:59:49 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: peter $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: debug.c,v 1.7 2004/06/12 08:59:49 peter Exp $";
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

static void
__debugnl (void)
{
  __os_print ("\r\n");
}

#define NL() __debugnl()
#define VAL(s,i) __debugval(s,i)

void
__debug (const char *s)
{
  struct __unixlib_fd *f;
  int i;

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
  if (!valid_address((const int *)&__u[0], (const int *)&__u[1]))
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
    for (i = 0; i < __u->argc; i++)
      {
        VAL ("argv[", (int) (__u->argv + i));
        VAL (" ", i);
        VAL (" ]:", (int) __u->argv[i]);
        __os_print (" ");
        __os_print (__u->argv[i]);
        NL ();
      }
  if (__u->envp)
    for (i = 0; i < __u->envc; i++)
      {
        VAL ("envp[", (int) (__u->envp + i));
        VAL (" ", i);
        VAL (" ]:", (int) __u->envp[i]);
        __os_print (" ");
        __os_print (__u->envp[i]);
        NL ();
      }
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
	      __os_print ("filename:");
	      __os_print (fname);
	      NL ();
	    }
	}
  VAL ("pid:", (int) __u->pid);
  VAL (" ppid:", (int) __u->ppid);
  VAL (" pproc:", (int) __u->pproc);
  NL ();
  VAL ("sigexit:", (int) __u->status.signal_exit);
  VAL (" core:", (int) __u->status.core_dump);
  VAL (" stopped:", (int) __u->status.stopped);
  NL ();
  VAL ("has_parent:", (int) __u->status.has_parent);
  NL ();
  VAL ("signal:", (int) __u->status.signal);
  VAL (" return:", (int) __u->status.return_code);
  VAL (" vreg:", (int) __u->vreg);
  NL ();
  NL ();
  /* os_get (); */
}
