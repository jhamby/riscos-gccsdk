/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/wait.c,v $
 * $Date: 2001/01/29 15:10:22 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: wait.c,v 1.2 2001/01/29 15:10:22 admin Exp $";
#endif

#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

/* Wait for a child to die. When one does, put its status in *status
   and return its process ID. For errors, return -1.  */

pid_t
wait (int *status)
{
  PTHREAD_SAFE_CANCELLATION

  return wait4 (WAIT_ANY, status, 0, 0);
}
