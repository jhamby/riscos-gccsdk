/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/sigsetjmp.c,v $
 * $Date: 2003/04/28 21:04:36 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigsetjmp.c,v 1.4 2003/04/28 21:04:36 alex Exp $";
#endif

#include <setjmp.h>
#include <unixlib/sigstate.h>
#include <unixlib/unix.h>
#include <pthread.h>

void __sigsetjmp_helper (sigjmp_buf state, int savesigs)
{
  PTHREAD_UNSAFE

  if (savesigs)
    {
      state->__mask_was_saved = 1;
      state->saved_mask = __u->sigstate.blocked;
      state->saved_currently_handling = __u->sigstate.currently_handling;
    }
  else
    state->__mask_was_saved = 0;
}

void __siglongjmp_helper (const sigjmp_buf state, int value)
{
  PTHREAD_UNSAFE

  value = value;

  if (state->__mask_was_saved)
    {
      __u->sigstate.blocked = state->saved_mask;
      __u->sigstate.currently_handling = state->saved_currently_handling;
      /* Cause delivery of some pending signals.  */
      __unixlib_raise_signal (0, 0);
    }
}
