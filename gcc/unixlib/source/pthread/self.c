/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/self.c,v $
 * $Date: 2002/12/15 13:16:55 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: self.c,v 1.1 2002/12/15 13:16:55 admin Exp $";
#endif

/* Written by Martin Piper and Alex Waugh */

#include <pthread.h>

/* Return the thread id of the currently running thread */
pthread_t
pthread_self (void)
{
  return __pthread_running_thread;
}
