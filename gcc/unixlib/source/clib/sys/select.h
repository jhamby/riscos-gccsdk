/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/select.h,v $
 * $Date: 2002/12/22 18:22:28 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* POSIX 1003.1g: 6.2 Select from File Descriptor Sets */

#ifndef __SYS_SELECT_H
#define __SYS_SELECT_H

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#define __need_timeval
#include <sys/time.h>

#define __need_timespec
#include <time.h>

__BEGIN_DECLS

/* Number of descriptors that can fit in an `fd_set'.  */
#define	FD_SETSIZE	256

/* It's easier to assume 8-bit bytes than to get CHAR_BIT.  */
#define	NFDBITS	(sizeof (unsigned long int) * 8)
#define	FDELT(d)	((d) / NFDBITS)
#define	FDMASK(d)	(1ul << ((d) % NFDBITS))

typedef unsigned long int __fd_mask;

typedef struct
  {
    /* Some braindead old software uses this member name.  */
    __fd_mask fds_bits[(FD_SETSIZE + (NFDBITS - 1)) / NFDBITS];
  } __fd_set;

#define	FD_ZERO(set) ((void) memset (set, 0, sizeof (*(set))))
#define	FD_SET(d, set)	((set)->fds_bits[FDELT(d)] |= FDMASK(d))
#define	FD_CLR(d, set)	((set)->fds_bits[FDELT(d)] &= ~FDMASK(d))
#define	FD_ISSET(d, set)	((set)->fds_bits[FDELT(d)] & FDMASK(d))


typedef __fd_mask fd_mask;
typedef __fd_set fd_set;

/* This contains the prototype for select */
#define howmany(x, y) (((x)+((y)-1))/(y))

/* Check the first NFDS descriptors each in READFDS (if not NULL) for read
   readiness, in WRITEFDS (if not NULL) for write readiness, and in EXCEPTFDS
   (if not NULL) for exceptional conditions.  If TIMEOUT is not NULL, time out
   after waiting the interval specified therein.  Returns the number of ready
   descriptors, or -1 for errors.  */

int select (int __nfds, __fd_set *__readfds, __fd_set *__writefds,
	    __fd_set *__exceptfds, struct timeval *__timeout) __THROW;

/* Same as pselect but with higher resolution for the timeout.  */
int pselect (int __nfds, __fd_set *__readfds, __fd_set *__writefds,
	     __fd_set *__exceptfds, const struct timespec *__timeout,
	     const __sigset_t *__sigmask) __THROW;

#ifdef __UNIXLIB_INTERNALS
/* SWI veneer. Do not use directly.  */
int _select (int __nfds, __fd_set *__readfds, __fd_set *__writefds,
	     __fd_set *__exceptfds, const struct timeval *__timeout) __THROW;
#endif  /* __UNIXLIB_INTERNALS */

__END_DECLS

#endif
