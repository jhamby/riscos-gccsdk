/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/wait.c,v $
 * $Date: 2003/04/05 09:33:57 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

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
