/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/signal/c/pause,v $
 * $Date: 1997/10/09 20:00:28 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: pause,v 1.5 1997/10/09 20:00:28 unixlib Exp $";
#endif

/* signal.c.pause: Written by Nick Burrett, 5 October 1996.  */

#include <signal.h>
#include <unistd.h>


/* Suspend the process until a signal arrives.
   This always returns -1 and sets errno to EINTR.  */

int
pause (void)
{
  /* errno is set to EINTR by sigsuspend() which is called
     through sigpause().  */
  return sigpause (sigblock (0));
}
