/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/sys/h/time,v $
 * $Date: 1997/10/09 19:59:57 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifndef __SYS_TIME_H
#define __SYS_TIME_H 1

#ifdef __cplusplus
extern "C" {
#endif

/* A time value that is accurate to the nearest
   microsecond but also has a range of years.  */
struct timeval
  {
    int tv_sec;			/* Seconds.  */
    int tv_usec;		/* Microseconds.  */
  };

/* POSIX.4 structure for a time value.  This is like a `struct timeval' but
   has nanoseconds instead of microseconds.  */
struct timespec
  {
    long int ts_sec;		/* Seconds.  */
    long int ts_nsec;		/* Nanoseconds.  */
  };

/* Macros for converting between `struct timeval' and `struct timespec'.  */
#define TIMEVAL_TO_TIMESPEC(tv, ts) {                                   \
        (ts)->ts_sec = (tv)->tv_sec;                                    \
        (ts)->ts_nsec = (tv)->tv_usec * 1000;                           \
}
#define TIMESPEC_TO_TIMEVAL(tv, ts) {                                   \
        (tv)->tv_sec = (ts)->ts_sec;                                    \
        (tv)->tv_usec = (ts)->ts_nsec / 1000;                           \
}


/* Structure crudely representing a timezone.
   This is obsolete and should never be used.  */
struct timezone
  {
    int tz_minuteswest;		/* Minutes west of GMT.  */
    int tz_dsttime;		/* Nonzero if DST is ever in effect.  */
  };

/* Get the current time of day and timezone information,
   putting it into *TV and *TZ.  If TZ is NULL, *TZ is not filled.
   Returns 0 on success, -1 on errors.
   NOTE: This form of timezone information is obsolete.
   Use the functions and variables declared in <time.h> instead.  */
extern int gettimeofday (struct timeval *, struct timezone *);

/* Set the current time of day and timezone information.
   This call is restricted to the super-user.  */
extern int settimeofday (const struct timeval *, const struct timezone *);

/* Adjust the current time of day by the amount in DELTA.
   If OLDDELTA is not NULL, it is filled in with the amount
   of time adjustment remaining to be done from the last `adjtime' call.
   This call is restricted to the super-user.  */
extern int adjtime (const struct timeval *, struct timeval *);


/* Values for the first argument to `getitimer' and `setitimer'.  */
enum __itimer_which
  {
    /* Timers run in real time.  */
    ITIMER_REAL = 0,
    /* Timers run only when the process is executing.  */
    ITIMER_VIRTUAL = 1,
    /* Timers run when the process is executing and when
       the system is executing on behalf of the process.  */
    ITIMER_PROF = 2,
    /* Used in <sys/unix.h>.  */
    __MAX_ITIMERS = 3
  };

/* Type of the second argument to `getitimer' and
   the second and third arguments `setitimer'.  */
struct itimerval
  {
    /* Value to put into `it_value' when the timer expires.  */
    struct timeval it_interval;
    /* Time to the next timer expiration.  */
    struct timeval it_value;
  };

/* Set *VALUE to the current setting of timer WHICH.
   Return 0 on success, -1 on errors.  */
extern int getitimer (enum __itimer_which, struct itimerval *);

/* Set the timer WHICH to *NEW.  If OLD is not NULL,
   set *OLD to the old value of timer WHICH.
   Returns 0 on success, -1 on errors.  */
extern int setitimer (enum __itimer_which, const struct itimerval *new_timer,
		      struct itimerval *old_timer);

/* Change the access time of FILE to TVP[0] and
   the modification time of FILE to TVP[1].  */
extern int utimes (const char *, const struct timeval __tvp[2]);


/* Convenience macros for operations on timevals.
   NOTE: `timercmp' does not work for >= or <=.  */
#define	timerisset(tvp)		((tvp)->tv_sec || (tvp)->tv_usec)
#define	timercmp(tvp, uvp, CMP)	\
  ((tvp)->tv_sec CMP (uvp)->tv_sec || \
   (tvp)->tv_sec == (uvp)->tv_sec && (tvp)->tv_usec CMP (uvp)->tv_usec)
#define	timerclear(tvp)		((tvp)->tv_sec = (tvp)->tv_usec = 0)

#ifdef __cplusplus
	}
#endif


#endif /* sys/time.h */
