/* Get time.
   Copyright (c) 2003-2008 UnixLib Developers.  */

#include <errno.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>

#include <internal/os.h>
#include <unixlib/local.h>

/* The `gettimeofday' function returns the current date and time in
   the `struct timeval' structure indicated by TP.  Information about
   the time zone is returned in the structure pointed at TZP.  If the
   TZP argument is a NULL pointer, time zone information is ignored.

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
  unsigned int buf[2];
  _kernel_oserror *err;
  uint64_t centisec, sec;

  if (tv == NULL)
    return __set_errno (EINVAL);

  buf[0] = 3;
  if ((err = __os_word(14, buf)) != NULL)
    return __ul_seterr (err, 1);

  /* The number of centiseconds that have elapsed between the starts
     of RISC OS and Unix times is 0x336e996a00.  */
  centisec = (uint64_t)buf[0] + (((uint64_t)(buf[1] & 0xFF))<<32)
	     - 0x336e996a00ULL;

  sec = (centisec / 10ULL) / 10ULL;
  tv->tv_sec = (time_t)sec;
  tv->tv_usec = 10000 * (suseconds_t)(centisec - sec * 100ULL);

  if (tz != NULL)
    {
      tzset (); /* Initialises timezone & daylight.  */

      tz->tz_minuteswest = timezone / 60;
      tz->tz_dsttime = 0;
    }

  return 0;
}
