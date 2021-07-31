/* Written by Alex Waugh.
   Copyright (c) 2002, 2003, 2004, 2005, 2006, 2008 UnixLib Developers.  */

#include <pthread.h>
#include <stdlib.h>

#include <internal/os.h>

/* Called whenever there is an error that would not be sensible to continue
   running after. */
void
__pthread_fatal_error (const char *msg)
{
  __pthread_stop_ticker ();
  __os_print ("\r\npthreads: ***fatal error, aborting***\r\n");
  __os_print (msg);
  __os_nl ();
  abort ();
}
