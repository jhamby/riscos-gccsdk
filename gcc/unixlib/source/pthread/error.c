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

/* Written by Alex Waugh */

#include <pthread.h>
#include <stdlib.h>
#include <unixlib/os.h>


/* Called whenever there is an error that would not be sensible to continue running after */
void
__pthread_fatal_error (const char *msg)
{
  __pthread_stop_ticker ();
  __os_print ("\r\npthreads: ***fatal error, aborting***\r\n");
  __os_print (msg);
  __os_print ("\r\n");
  abort ();
}

