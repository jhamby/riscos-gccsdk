/* poll ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <string.h>
#include <stddef.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <sys/select.h>

#include <pthread.h>
#include <internal/unix.h>

/* Poll the file descriptors described by the NFDS structures starting at
   FDS.  If TIMEOUT is nonzero and not -1, allow TIMEOUT milliseconds for
   an event to occur; if TIMEOUT is -1, block until an event occurs.
   Returns the number of file descriptors with events, zero if timed out,
   or -1 for errors.  */

int
poll (struct pollfd *fds, nfds_t nfds, int timeout)
{
  struct timeval tv;
  fd_set rset, wset, xset;
  struct pollfd *f;
  int ready;
  int maxfd = 0;

  PTHREAD_SAFE_CANCELLATION

  FD_ZERO (&rset);
  FD_ZERO (&wset);
  FD_ZERO (&xset);

  for (f = fds; f < &fds[nfds]; ++f)
    if (f->fd >= 0)
      {
	if (f->events & POLLIN)
	  FD_SET (f->fd, &rset);
	if (f->events & POLLOUT)
	  FD_SET (f->fd, &wset);
	if (f->events & POLLPRI)
	  FD_SET (f->fd, &xset);
	if (f->fd > maxfd && (f->events & (POLLIN | POLLOUT | POLLPRI)))
	  maxfd = f->fd;
      }

  tv.tv_sec = timeout / 1000;
  tv.tv_usec = (timeout % 1000) * 1000;

  ready = select (maxfd + 1, &rset, &wset, &xset,
		  timeout == -1 ? NULL : &tv);
  if (ready > 0)
    for (f = fds; f < &fds[nfds]; ++f)
      {
	f->revents = 0;
	if (f->fd >= 0)
	  {
	    if (FD_ISSET (f->fd, &rset))
	      f->revents |= POLLIN;
	    if (FD_ISSET (f->fd, &wset))
	      f->revents |= POLLOUT;
	    if (FD_ISSET (f->fd, &xset))
	      f->revents |= POLLPRI;
	  }
      }

  return ready;
}
