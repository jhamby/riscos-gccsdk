/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/wait.c,v $
 * $Date: 2000/07/15 14:52:45 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: wait.c,v 1.1.1.1 2000/07/15 14:52:45 nick Exp $";
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
