/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/error.c,v $
 * $Date: 2002/12/15 13:16:55 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: error.c,v 1.1 2002/12/15 13:16:55 admin Exp $";
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
  __os_nl ();
  abort ();
}

