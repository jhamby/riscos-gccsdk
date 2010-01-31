/* pthread_equal ()
   Copyright (c) 2002-2007 UnixLib Developers
   Written by Martin Piper and Alex Waugh.  */

#include <pthread.h>


/* Return non zero if two pthread_t's refer to the same thread */ 
int
pthread_equal (pthread_t t1, pthread_t t2)
{
  return t1 == t2;
}
