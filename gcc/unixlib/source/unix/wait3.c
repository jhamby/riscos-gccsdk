/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/wait3.c,v $
 * $Date: 2001/01/29 15:10:22 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

/* Wait for a child to die. When one does, put its status in *status
   and return its process ID. For errors, return -1.  */

pid_t
wait3 (int *status, int options, struct rusage *usage)
{
  return wait4 (WAIT_ANY, status, options, usage);
}
