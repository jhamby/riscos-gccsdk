/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/gettod.c,v $
 * $Date: 2000/07/15 14:52:42 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: gettod.c,v 1.1.1.1 2000/07/15 14:52:42 nick Exp $";
#endif

#include <errno.h>
#include <time.h>
#include <sys/time.h>


/* The `gettimeofday' function returns the current date and time in
   the `struct timeval' structure indicated by TP.  Information about
   the time zone is returned in the structure pointed at TZP.  If the
   TZP argument is a null pointer, time zone information is ignored.

   The return value is `0' on success and `-1' on failure.  The
   following `errno' error condition is defined for this function:

   `ENOSYS'
   The operating system does not support getting time zone
   information, and TZP is not a null pointer.  The GNU
   operating system does not support using `struct timezone' to
   represent time zone information; that is an obsolete feature
   of 4.3 BSD.  */


/* Get the current time of day and timezone information,
   putting it into *TV and *TZ.  If TZ is NULL, *TZ is not filled.
   Returns 0 on success, -1 on errors.  */
int
gettimeofday (struct timeval *tv, struct timezone *tz)
{
  if (tv == NULL)
    {
      errno = EINVAL;
      return -1;
    }

  tv->tv_sec = time ((time_t *) NULL);
  tv->tv_usec = 0;

  if (tz != NULL)
    {
      const time_t timer = tv->tv_sec;
      const struct tm *tm;

      const int save_timezone = timezone;
      const struct tm saved_tz = __tz[0];

      tm = localtime (&timer);

      tz->tz_minuteswest = timezone / 60;
      tz->tz_dsttime = 0;

      timezone = save_timezone;
      __tz[0] = saved_tz;

      if (tm == NULL)
	return -1;
    }

  return 0;
}
