/* getitimer ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <stddef.h>
#include <errno.h>
#include <sys/time.h>

#include <internal/unix.h>

/* The `getitimer' function stores information about the timer
   specified by 'which' in the structure pointed at by 'value'

   The return value and error conditions are the same as for
   `setitimer'.

   Set *value to the current setting of timer 'which'.
   Return 0 on success, -1 on errors.  */
int
getitimer (__itimer_which_t which, struct itimerval *value)
{
  /* We can't implement interval timers whilst executing in a task
     window nor whilst running as a WIMP program.  */
  if (__get_taskhandle () != 0)
    return __set_errno (ENOSYS);

  if ((unsigned) which >= __MAX_ITIMERS)
    return __set_errno (EINVAL);

  *value = __u->itimers[which];

  return 0;
}
