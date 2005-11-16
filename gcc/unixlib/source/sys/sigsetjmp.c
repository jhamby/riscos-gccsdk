/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/sigsetjmp.c,v $
 * $Date: 2003/06/23 20:33:04 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

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
