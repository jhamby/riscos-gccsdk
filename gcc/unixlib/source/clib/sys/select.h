/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/select.h,v $
 * $Date: 2000/07/15 14:52:16 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* POSIX 1003.1g: 6.2 Select from File Descriptor Sets */

#ifndef __SYS_SELECT_H
#define __SYS_SELECT_H

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define	FD_SETSIZE	__FD_SETSIZE
#define	NFDBITS		__NFDBITS
#define	FD_ZERO(set)	__FD_ZERO(set)
#define	FD_SET(d, set)	__FD_SET((d), (set))
#define	FD_CLR(d, set)	__FD_CLR((d), (set))
#define	FD_ISSET(d, set)__FD_ISSET((d), (set))

#ifndef __SYS_TIME_H
/* This being here makes the `select' prototype valid whether or not
   we have already included <sys/time.h> to define `struct timeval'.  */
struct timeval;
struct timespec;
#endif

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
	    __fd_set *__exceptfds, struct timeval *__timeout);

/* Same as pselect but with higher resolution for the timeout.  */
int pselect (int __nfds, __fd_set *__readfds, __fd_set *__writefds,
	    __fd_set *__exceptfds, struct timespec *__timeout);

#ifdef __UNIXLIB_INTERNALS
/* SWI veneer. Do not use directly.  */
int _select (int __nfds, __fd_set *__readfds, __fd_set *__writefds,
	     __fd_set *__exceptfds, const struct timeval *__timeout);
#endif  /* __UNIXLIB_INTERNALS */


#ifdef __cplusplus
}
#endif

#endif
