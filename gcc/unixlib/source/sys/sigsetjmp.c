/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/sys/c/sigsetjmp,v $
 * $Date: 1997/10/09 20:00:40 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigsetjmp,v 1.5 1997/10/09 20:00:40 unixlib Exp $";
#endif

#include <setjmp.h>
#include <unixlib/sigstate.h>
#include <sys/unix.h>

void __sigsetjmp_helper (sigjmp_buf state, int savesigs)
{
  if (savesigs)
    {
      state->__mask_was_saved = 1;
      state->saved_mask = __u->sigstate.blocked;
    }
  else
    state->__mask_was_saved = 0;
}

void __siglongjmp_helper (const sigjmp_buf state, int value)
{
  value = value;

  if (state->__mask_was_saved)
    {
      __u->sigstate.blocked = state->saved_mask;
      /* Cause delivery of some pending signals.  */
      __unixlib_raise_signal (0, 0);
    }
}
