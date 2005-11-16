/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/self.c,v $
 * $Date: 2003/04/05 12:42:28 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

/* Written by Martin Piper and Alex Waugh */

#include <pthread.h>

/* Return the thread id of the currently running thread */
pthread_t
pthread_self (void)
{
  return __pthread_running_thread;
}
