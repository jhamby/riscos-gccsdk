/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/signal/c/sigaction,v $
 * $Date: 1997/10/09 20:00:28 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigaction,v 1.6 1997/10/09 20:00:28 unixlib Exp $";
#endif

/* sigaction.c: Written by Nick Burrett, 31 August 1996.  */

#include <errno.h>
#include <stddef.h>
#include <signal.h>
#include <unixlib/sigstate.h>
#include <sys/unix.h>

/* Set up a new action for the signal 'sig' specified by 'act'.
   The old action is returned in 'oact'.

   If oact is null, no information is returned.
   If act is null, the signal action remains unchanged, you can then
   read the action currently associated with the signal.

   Returns zero on sucess, -1 on failure.  */

int
sigaction (int sig, const struct sigaction *act, struct sigaction *oact)
{
  struct unixlib_sigstate *ss = &__u->sigstate;

  if (__SIG_INVALID_P (sig) ||
      (act != NULL && act->sa_handler != SIG_DFL &&
       (sig == SIGKILL || sig == SIGSTOP)))
      /* Signal number was out of range, or a SIGKILL or SIGSTOP
         was trapped.  */
    return __set_errno (EINVAL);

  if (oact != NULL)
    {
      oact->sa_handler = ss->actions[sig].sa_handler;
      oact->sa_mask = ss->actions[sig].sa_mask;
      oact->sa_flags = ss->actions[sig].sa_flags;
    }

  if (act != NULL)
    {
      ss->actions[sig].sa_handler = act->sa_handler;
      ss->actions[sig].sa_mask = act->sa_mask;
      ss->actions[sig].sa_flags = act->sa_flags;
    }

  return 0;
}
