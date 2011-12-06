/*
 * POSIX 1003.1g: 6.2 Select from File Descriptor Sets
 * Copyright (c) 2000-2011 UnixLib Developers
 */

#ifndef __SYS_SELECT_H
#define __SYS_SELECT_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#define __need_time_t
#define __need_timespec
#include <time.h>

#define __need_timeval
#include <sys/time.h>

#ifndef __suseconds_t_defined
typedef __suseconds_t suseconds_t;
#define __suseconds_t_defined
#endif

__BEGIN_DECLS

/* Number of descriptors that can fit in an `fd_set'.  */
#define	FD_SETSIZE	__FD_SETSIZE

typedef long int __fd_mask;

/* It's easier to assume 8-bit bytes than to get CHAR_BIT.  */
#define	__NFDBITS	(sizeof (__fd_mask) * 8)
#define	__FDELT(d)	((d >= __FD_SOCKET_OFFSET ? d - __FD_SOCKET_OFFSET : d) / __NFDBITS)
#define	__FDMASK(d)	(1 << ((d >= __FD_SOCKET_OFFSET ? d - __FD_SOCKET_OFFSET : d) % __NFDBITS))

typedef struct
  {
    /* Some braindead old software uses this member name.  */
    __fd_mask fds_bits[(FD_SETSIZE + (__NFDBITS - 1)) / __NFDBITS];
  } __fd_set;

#define	FD_ZERO(set) ((void) memset (set, 0, sizeof (*(set))))
#define	FD_SET(d, set)	((set)->fds_bits[__FDELT(d)] |= __FDMASK(d))
#define	FD_CLR(d, set)	((set)->fds_bits[__FDELT(d)] &= ~__FDMASK(d))
#define	FD_ISSET(d, set)	((set)->fds_bits[__FDELT(d)] & __FDMASK(d))

#ifdef __USE_MISC
/* Sometimes the fd_set member is assumed to have this type.  */
typedef __fd_mask fd_mask;

/* Number of bits per word of `fd_set' (some code assumes this is 32).  */
# define NFDBITS		__NFDBITS
#endif

typedef __fd_set fd_set;

/* This contains the prototype for select */
#define howmany(x, y) (((x)+((y)-1))/(y))

/* Check the first NFDS descriptors each in READFDS (if not NULL) for read
   readiness, in WRITEFDS (if not NULL) for write readiness, and in EXCEPTFDS
   (if not NULL) for exceptional conditions.  If TIMEOUT is not NULL, time out
   after waiting the interval specified therein.  Returns the number of ready
   descriptors, or -1 for errors.

   This function is a cancellation point.  */
extern int select (int __nfds, __fd_set *__restrict __readfds,
		   fd_set *__restrict __writefds,
		   fd_set *__restrict __exceptfds,
		   struct timeval *__restrict __timeout);

#ifndef __TARGET_SCL__
/* Same as pselect but with higher resolution for the timeout.
   This function is a cancellation point.  */
extern int pselect (int __nfds,
		    fd_set *__restrict __readfds,
		    fd_set *__restrict __writefds,
		    fd_set *__restrict __exceptfds,
		    const struct timespec *__restrict __timeout,
		    const __sigset_t *__restrict __sigmask);
#endif

__END_DECLS

#endif
