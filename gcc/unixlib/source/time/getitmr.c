/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/getitmr.c,v $
 * $Date: 2000/07/15 14:52:42 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getitmr.c,v 1.1.1.1 2000/07/15 14:52:42 nick Exp $";
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
     window.  */
  if (__taskwindow)
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
