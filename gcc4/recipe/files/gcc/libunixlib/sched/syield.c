/* Yield control to another process.
   Copyright (c) 2005 UnixLib Developers.  */

#include <errno.h>
#include <sched.h>
#include <pthread.h>

int sched_yield (void)
{
  /* Not sure this is correct, but it is better than a kick in the teeth.  */
  pthread_yield ();
  return 0;
}
