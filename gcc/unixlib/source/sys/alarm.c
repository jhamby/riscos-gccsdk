/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/alarm.c,v $
 * $Date: 2000/07/15 14:52:32 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: alarm.c,v 1.1.1.1 2000/07/15 14:52:32 nick Exp $";
#endif

#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <unixlib/features.h>

unsigned int
alarm (unsigned int seconds)
{
#if __FEATURE_ITIMERS
  struct itimerval old, new_timer;
  new_timer.it_interval.tv_usec = 0;
  new_timer.it_interval.tv_sec = 0;
  new_timer.it_value.tv_usec = 0;
  new_timer.it_value.tv_sec = (int) seconds;
  return (setitimer (ITIMER_REAL, &new_timer, &old) < 0) ? 0 : old.it_value.tv_sec;
#else
  return __set_errno (ENOSYS);
#endif
}
