/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/wait,v $
 * $Date: 1997/10/09 20:00:53 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: wait,v 1.5 1997/10/09 20:00:53 unixlib Exp $";
#endif

#include <sys/wait.h>
#include <sys/types.h>

/* Wait for a child to die. When one does, put its status in *status
   and return its process ID. For errors, return -1.  */

pid_t
wait (int *status)
{
  return wait4 (WAIT_ANY, status, 0, 0);
}
