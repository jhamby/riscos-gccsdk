/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/poll.h,v $
 * $Date: 2000/07/15 14:52:15 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* System V poll interface.  */

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: poll.h,v 1.1.1.1 2000/07/15 14:52:15 nick Exp $";
#endif

#ifndef __SYS_POLL_H
#define __SYS_POLL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Data structure describing a polling request.  */
struct pollfd
  {
    /* File descriptor to poll.  */
    int fd;
    /* Types of events poller cares about.  */
    short int events;
    /* Types of events that actually occurred.  */
    short int revents;
  };

/* Event types that can be polled for.   */

/* Data to be read.  */
#define POLLIN		01
/* Urgent data to read.  */
#define POLLPRI		02
/* Writing now will not block.  */
#define POLLOUT		04

/* Event types always implicitly polled for.  */

/* Error condition.  */
#define POLLERR         010
/* Hung up.  */
#define POLLHUP         020
/* Invalid polling request.  */
#define POLLNVAL        040

/* Canonical number of polling requests to read in at a time in poll.  */
#define NPOLLFILE	30


/* Poll the file descriptors described by the NFDS structures starting at
   FDS.  If TIMEOUT is nonzero and not -1, allow TIMEOUT milliseconds for
   an event to occur; if TIMEOUT is -1, block until an event occurs.
   Returns the number of file descriptors with events, zero if timed out,
   or -1 for errors.  */

extern int poll (struct pollfd *__fds, unsigned long int __nfds,
		 int __timeout);

#ifdef __cplusplus
}
#endif

#endif
