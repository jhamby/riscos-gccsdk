/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/time/c/getitmr,v $
 * $Date: 2000/07/03 11:32:44 $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getitmr,v 1.8 2000/07/03 11:32:44 admin Exp $";
#endif

#include <stddef.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/unix.h>
#include <sys/syslib.h>

/* The `getitimer' function stores information about the timer
   specified by 'which' in the structure pointed at by 'value'

   The return value and error conditions are the same as for
   `setitimer'.

   Set *value to the current setting of timer 'which'.
   Return 0 on success, -1 on errors.  */
int
getitimer (enum __itimer_which which, struct itimerval *value)
{
  /* We can't implement interval timers whilst executing in a task
     window nor whilst running as a WIMP program.  Note that when
     __taskwindow == 1 => __wimpprogram == 1 but not necessairy vice
     versa so the test on __wimpprogram is enough. */
  if (__wimpprogram)
    {
      errno = ENOSYS;
      return -1;
    }

  if ((unsigned) which >= __MAX_ITIMERS)
    {
      errno = EINVAL;
      return -1;
    }
  *value = __u->itimers[which];

  return 0;
}
