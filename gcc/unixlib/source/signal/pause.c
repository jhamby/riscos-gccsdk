/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/pause.c,v $
 * $Date: 2001/01/29 15:10:20 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: pause.c,v 1.2 2001/01/29 15:10:20 admin Exp $";
#endif

/* signal.c.pause: Written by Nick Burrett, 5 October 1996.  */

#include <signal.h>
#include <unistd.h>
#include <pthread.h>


/* Suspend the process until a signal arrives.
   This always returns -1 and sets errno to EINTR.  */

int
pause (void)
{
  PTHREAD_SAFE_CANCELLATION

  /* errno is set to EINTR by sigsuspend() which is called
     through sigpause().  */
  return sigpause (sigblock (0));
}
