/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/asctime.c,v $
 * $Date: 2003/04/05 09:33:56 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#include <errno.h>
#include <time.h>

int clock_settime (clockid_t clk_id, const struct timespec *tp)
{
  switch (clk_id)
    {
    case CLOCK_REALTIME:
    case CLOCK_MONOTONIC:
      return __set_errno (EPERM);
      break;
    default:
      errno = EINVAL;
      return -1;
    }
  return 0;
}
