/*
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef __SYS_TIME_H

#if ! defined __need_timeval
#define __SYS_TIME_H

#define __need_timeval
#endif

#if !defined __timeval_defined && defined __need_timeval
#define __timeval_defined 1
#include <unixlib/types.h>
/* A time value that is accurate to the nearest
   microsecond but also has a range of years.  */
struct timeval
  {
    __time_t tv_sec;		/* Seconds.  */
    __suseconds_t tv_usec;	/* Microseconds.  */
  };
#endif
#undef __need_timeval

#ifdef __SYS_TIME_H

#define __need_time_t
#include <time.h>
#include <sys/select.h>

#ifndef __suseconds_t_defined
typedef __suseconds_t suseconds_t;
#define __suseconds_t_defined
#endif

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

__BEGIN_DECLS

#ifdef __USE_GNU
/* Macros for converting between `struct timeval' and `struct timespec'.  */
#define TIMEVAL_TO_TIMESPEC(tv, ts) {                                   \
        (ts)->tv_sec = (tv)->tv_sec;                                    \
        (ts)->tv_nsec = (tv)->tv_usec * 1000;                           \
}
#define TIMESPEC_TO_TIMEVAL(tv, ts) {                                   \
        (tv)->tv_sec = (ts)->tv_sec;                                    \
        (tv)->tv_usec = (ts)->tv_nsec / 1000;                           \
}
#endif

#ifdef __USE_BSD
/* Structure crudely representing a timezone.
   This is obsolete and should never be used.  */
struct timezone
  {
    int tz_minuteswest;		/* Minutes west of GMT.  */
    int tz_dsttime;		/* Nonzero if DST is ever in effect.  */
  };

typedef struct timezone *__restrict __timezone_ptr_t;
#else
typedef void *__restrict __timezone_ptr_t;
#endif

/* Get the current time of day and timezone information,
   putting it into *TV and *TZ.  If TZ is NULL, *TZ is not filled.
   Returns 0 on success, -1 on errors.
   NOTE: This form of timezone information is obsolete.
   Use the functions and variables declared in <time.h> instead.  */
extern int gettimeofday (struct timeval *__restrict __tv,
			 __timezone_ptr_t __tz) __THROW;

#ifdef __USE_BSD
/* Set the current time of day and timezone information.
   This call is restricted to the super-user.  */
extern int settimeofday (const struct timeval *__tv,
			 const struct timezone *__tz) __THROW;

/* Adjust the current time of day by the amount in DELTA.
   If OLDDELTA is not NULL, it is filled in with the amount
   of time adjustment remaining to be done from the last `adjtime' call.
   This call is restricted to the super-user.  */
extern int adjtime (const struct timeval *, struct timeval *) __THROW;
#endif

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
    /* Used in <internal/unix.h>.  */
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
extern int getitimer (enum __itimer_which, struct itimerval *) __THROW;

/* Set the timer WHICH to *NEW.  If OLD is not NULL,
   set *OLD to the old value of timer WHICH.
   Returns 0 on success, -1 on errors.  */
extern int setitimer (enum __itimer_which, const struct itimerval *__new_timer,
		      struct itimerval *__old_timer) __THROW;

/* Change the access time of FILE to TVP[0] and
   the modification time of FILE to TVP[1].  */
extern int utimes (const char *, const struct timeval __tvp[2]) __THROW;

#ifdef __USE_BSD
/* Convenience macros for operations on timevals.
   NOTE: `timercmp' does not work for >= or <=.  */
#define	timerisset(tvp)		((tvp)->tv_sec || (tvp)->tv_usec)
#define	timerclear(tvp)		((tvp)->tv_sec = (tvp)->tv_usec = 0)

#define	timercmp(tvp, uvp, CMP)	\
  ((tvp)->tv_sec CMP (uvp)->tv_sec || \
   (tvp)->tv_sec == (uvp)->tv_sec && (tvp)->tv_usec CMP (uvp)->tv_usec)

# define timeradd(a, b, result)                                               \
  do {                                                                        \
    (result)->tv_sec = (a)->tv_sec + (b)->tv_sec;                             \
    (result)->tv_usec = (a)->tv_usec + (b)->tv_usec;                          \
    if ((result)->tv_usec >= 1000000)                                         \
      {                                                                       \
        ++(result)->tv_sec;                                                   \
        (result)->tv_usec -= 1000000;                                         \
      }                                                                       \
  } while (0)

# define timersub(a, b, result)                                               \
  do {                                                                        \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;                             \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;                          \
    if ((result)->tv_usec < 0) {                                              \
      --(result)->tv_sec;                                                     \
      (result)->tv_usec += 1000000;                                           \
    }                                                                         \
  } while (0)

#endif

__END_DECLS

#endif /* sys/time.h */
#else
#undef __need_timeval
#endif /* ! __SYS_TIME_H */
