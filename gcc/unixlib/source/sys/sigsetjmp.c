/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/sigsetjmp.c,v $
 * $Date: 2002/02/14 15:56:37 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigsetjmp.c,v 1.3 2002/02/14 15:56:37 admin Exp $";
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
      /* Cause delivery of some pending signals.  */
      __unixlib_raise_signal (0, 0);
    }
}
