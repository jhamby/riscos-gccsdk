/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/signal/c/sigvec,v $
 * $Date: 1997/10/09 20:00:30 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigvec,v 1.6 1997/10/09 20:00:30 unixlib Exp $";
#endif

/* c.sigvec: Written by Nick Burrett, 30 August 1996.  */

#include <signal.h>
#include <errno.h>
#include <stddef.h>


/* We use a wrapper handler to support SV_RESETHAND.  */

static __sighandler_t wrapped_handlers[NSIG];
static sigset_t wrapped_masks[NSIG];

static void
wrapper_handle (int sig)
{
  int save;
  struct sigaction act;

  act.sa_handler = SIG_DFL;
  act.sa_mask = wrapped_masks[sig];
  act.sa_flags = 0;
  save = errno;
  (void) sigaction (sig, &act, (struct sigaction *) NULL);
  (void) __set_errno (save);

  (*wrapped_handlers[sig]) (sig);
}


static int
convert_mask (sigset_t * set, const int mask)
{
  register int sig;

  if (set == NULL || sigemptyset (set) < 0)
    return __set_errno (EINVAL);

  for (sig = 1; sig < NSIG; ++sig)
    if (mask & sigmask (sig))
      if (sigaddset (set, sig) < 0)
	return -1;

  return 0;
}

/* If VEC is non-NULL, set the handler for SIG to the `sv_handler' member
   of VEC.  The signals in `sv_mask' will be blocked while the handler runs.
   If the SV_RESETHAND bit is set in `sv_flags', the handler for SIG will be
   reset to SIG_DFL before `sv_handler' is entered.  If OVEC is non-NULL,
   it is filled in with the old information for SIG.  */
int
sigvec (int sig, const struct *vec, struct sigvec *ovec)
{
  struct sigaction old;

  if (vec == NULL || !(vec->sv_flags & SV_RESETHAND))
    {
      struct sigaction new, *n;

      if (vec == NULL)
	n = NULL;
      else
	{
	  n = &new;
	  n->sa_handler = vec->sv_handler;
	  if (convert_mask (&n->sa_mask, vec->sv_mask) < 0)
	    return -1;
	  n->sa_flags = 0;

	  if (vec->sv_flags & SV_ONSTACK)
	    {
	      n->sa_flags |= SA_ONSTACK;
	    }
	  if (!(vec->sv_flags & SV_INTERRUPT))
	    n->sa_flags |= SA_RESTART;
	}

      if (sigaction (sig, n, &old) < 0)
	return -1;
    }
  else
    {
      struct sigaction wrapper;

      wrapper.sa_handler = wrapper_handler;
      wrapped_handlers[sig] = vec->sv_handler;
      if (convert_mask (&wrapped_masks[sig], vec->sv_mask) < 0)
	return -1;

      if (sigaction (sig, &wrapper, &old) < 0)
	return -1;
    }

  if (ovec != NULL)
    {
      register int i;
      int mask = 0;

      for (i = 1; i < NSIG; ++i)
	if (sigismember (&old.sa_mask, i))
	  mask |= sigmask (i);

      ovec->sv_mask = mask;
      ovec->sv_flags = 0;

      if (old.sa_flags & SA_ONSTACK)
	ovec->sv_flags |= SV_ONSTACK;
      if (!(old.sa_flags & SA_RESTART))
	ovec->sv_flags |= SV_INTERRUPT;
      if (old.sa_handler == wrapper_handler)
	{
	  ovec->sv_flags |= SV_RESETHAND;
	  ovec->sv_handler = wrapped_handlers[sig];
	}
      else
	ovec->sv_handler = old.sa_handler;
    }

  return 0;
}
