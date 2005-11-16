/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/pause.c,v $
 * $Date: 2003/04/28 21:04:36 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

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
