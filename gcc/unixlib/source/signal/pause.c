/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/pause.c,v $
 * $Date: 2000/07/15 14:52:29 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: pause.c,v 1.1.1.1 2000/07/15 14:52:29 nick Exp $";
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
