/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/equal.c,v $
 * $Date: 2002/12/15 13:16:55 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* Written by Martin Piper and Alex Waugh */

#include <pthread.h>


/* Return non zero if two pthread_t's refer to the same thread */ 
int
pthread_equal (pthread_t t1, pthread_t t2)
{
  return (t1 == t2);
}

