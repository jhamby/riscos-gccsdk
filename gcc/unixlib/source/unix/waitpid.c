/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/waitpid,v $
 * $Date: 1997/10/09 20:00:54 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: waitpid,v 1.4 1997/10/09 20:00:54 unixlib Exp $";
#endif

#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stddef.h>

/* Wait for a child matching pid to die.

   If pid > 0, match any process whose process ID is pid.
   If pid = -1, match any process.
   If pid = 0, match any process with the same process group
   as the current process.
   If pid < -1, match any process whose process group is the
   absolute value of pid.

   If the WNOHANG bit is set in options, and that child
   is not already dead, return (pid_t) 0.

   If successful, return pid and store the dead child's status in stat_loc.

   Return -1 for errors.

   If the WUNTRACED bit is set in OPTIONS, return status for stopped
   children; otherwise don't.  */

pid_t
waitpid (pid_t pid, int *stat_loc, int options)
{
  return wait4 (pid, stat_loc, options, NULL);
}
