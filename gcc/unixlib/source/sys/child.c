/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/sys/c/child,v $
 * $Date: 1997/10/09 20:00:39 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: child,v 1.4 1997/10/09 20:00:39 unixlib Exp $";
#endif

#include <sys/unix.h>
#include <sys/wait.h>

/* This is a utility function used by fork and vfork. It
   searches through the number of children this process
   can have and returns an index to a child suitable for
   re-birth.

   A child applicable for re-birth could either be one that
   has terminated or one that was never born in the first
   place.

   An unborn child will have no memory allocated to it.  */

int
__find_free_child (void)
{
  int i;

  for (i = 0; i < CHILD_MAX; i++)
    {
      /* A terminated child will be anything that hasn't stopped.  */
      if (!__u->child[i].status.stopped)
	return i;
    }
  return -1;
}
