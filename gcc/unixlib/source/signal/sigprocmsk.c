/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/signal/c/sigprocmsk,v $
 * $Date: 2000/06/03 14:46:10 $
 * $Revision: 1.11 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigprocmsk,v 1.11 2000/06/03 14:46:10 admin Exp $";
#endif

/* signal.c.sigprocmsk: Implementation of the POSIX signal function
   sigprocmask.

   Written by Nick Burrett, 5 October 1996.  */

#include <errno.h>
#include <signal.h>
#include <sys/unix.h>

/* #define DEBUG */

#ifdef DEBUG
#include <sys/os.h>
#endif

/* If SET is not NULL, modify the current set of blocked signals
   according to HOW, which may be SIG_BLOCK, SIG_UNBLOCK or SIG_SETMASK.
   If OSET is not NULL, store the old set of blocked signals in *OSET.

   If invoking sigprocmask causes any pending signals to be unblocked,
   at least one of those signals will be delivered to the process
   before sigprocmask returns.  The order in which pending signals
   are delivered is not specified.  */

int
sigprocmask (int how, const sigset_t * nset, sigset_t * oset)
{
  sigset_t mask, set;

  if (oset != NULL)
    *oset = __u->sigstate.blocked;

  if (nset == NULL)
    return 0;

  set = *nset;

  /* Don't block SIGKILL or SIGSTOP.  */
  sigdelset (&set, SIGKILL);
  sigdelset (&set, SIGSTOP);

  mask = (unsigned int) set;

#ifdef DEBUG
  os_print ("sigprocmask: how = "); os_prdec (how);
  os_print (", mask = "); os_prhex (mask); os_print ("\r\n");
#endif
  if (how == SIG_BLOCK)
    {
#ifdef DEBUG
      os_print ("sigprocmask: SIG_BLOCK\r\n");
#endif
      /* This will always block more signals,
         so we don't have to worry about delivering
         pending signals.  */
      __u->sigstate.blocked |= mask;
    }
  else if (how == SIG_UNBLOCK)
    {
#ifdef DEBUG
      os_print ("sigprocmask: SIG_UNBLOCK\r\n");
#endif
      __u->sigstate.blocked &= ~mask;
      /* Cause delivery of some pending signals.  */
      __unixlib_raise_signal (0, 0);
    }
  else if (how == SIG_SETMASK)
    {
#ifdef DEBUG
      os_print ("sigprocmask: SIG_SETMASK\r\n");
#endif
      __u->sigstate.blocked = mask;
      /* The new mask might have unblocked a few signals so try and
         deliver the pending ones.  */
      __unixlib_raise_signal (0, 0);
    }
  else
    {
#ifdef DEBUG
      os_print ("sigprocmask: invalid\r\n");
#endif
      return __set_errno (EINVAL);
    }
  return 0;
}
