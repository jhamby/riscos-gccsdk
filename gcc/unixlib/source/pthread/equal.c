/****************************************************************************
 *
 * $Source: $
 * $Date: $
 * $Revision: $
 * $State: $
 * $Author: $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: $";
#endif

/* Written by Martin Piper and Alex Waugh */

#include <pthread.h>


/* Return non zero if two pthread_t's refer to the same thread */ 
int
pthread_equal (pthread_t t1, pthread_t t2)
{
  return (t1 == t2);
}

