/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/pselect,v $
 * $Date: 1997/10/10 19:38:57 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: pselect,v 1.1 1997/10/10 19:38:57 unixlib Exp $";
#endif

#include <sys/time.h>
#include <sys/select.h>

int pselect (int nfds, fd_set *readfds, fd_set *writefds,
    	     fd_set *exceptfds, struct timespec *spec_timeout)
{
  struct timeval val_timeout;

  /* Convert the nanoseconds resolution into milliseconds.  */
  TIMESPEC_TO_TIMEVAL (&val_timeout, spec_timeout);
  return select (nfds, readfds, writefds, exceptfds, &val_timeout);
}
