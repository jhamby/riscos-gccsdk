/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/sys/c/alarm,v $
 * $Date: 1999/11/16 13:26:49 $
 * $Revision: 1.8 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: alarm,v 1.8 1999/11/16 13:26:49 admin Exp $";
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
