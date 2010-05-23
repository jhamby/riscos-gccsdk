/* select () for SCL
 * Copyright 2010 UnixLib Developers.
 */

#include <sys/select.h>

/* FIXME: only supports socket fd ! */

int
select (int nfds, fd_set *readfds, fd_set *writefds,
	fd_set *exceptfds, struct timeval *timeout)
{
  return _sselect (nfds, readfds, writefds, exceptfds, timeout);
}
