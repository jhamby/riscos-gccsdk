/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigstack.c,v $
 * $Date: 2000/07/15 14:52:30 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigstack.c,v 1.1.1.1 2000/07/15 14:52:30 nick Exp $";
#endif

/* sigstack.c: Written by Nick Burrett, 27 August 1996.
   This is the BSD version of the POSIX function sigaltstack.  */

#include <signal.h>
#include <errno.h>
#include <unixlib/sigstate.h>
#include <sys/unix.h>

/* Specifies an alternate stack for use during signal handling.
   When a signal is received by the process and its action
   indicates that the signal stack is used, the system arranges
   a switch to the currently installed signal stack while the
   handler for that signal is executed.  */

int
sigstack (const struct sigstack *stack, struct sigstack *oldstack)
{
  struct unixlib_sigstate *ss = &__u->sigstate;
  if (stack != NULL && ss->signalstack.ss_flags & SA_ONSTACK)
      /* Trying to disable a stack that is currently in use.  */
    return __set_errno (EINVAL);

  if (oldstack != NULL)
    {
      oldstack->ss_sp = ss->signalstack.ss_sp;
      oldstack->ss_onstack = ss->signalstack.ss_flags & SA_ONSTACK;
    }
  /* Converting between a sigstack and a sigaltstack.  With sigstack,
     ss_sp points to the top of the buffer because we have a downwards
     growing stack. Since we don't have a size parameter, we'll set
     size to -1 to tell the signal handler that ss_sp points to the
     top of the buffer, instead of the bottom.  */

  if (stack != NULL)
    {
      ss->signalstack.ss_sp = stack->ss_sp;
      ss->signalstack.ss_size = -1;
      ss->signalstack.ss_flags = stack->ss_onstack;
    }

  return 0;
}
