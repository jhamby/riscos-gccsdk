/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/getdtables.c,v $
 * $Date: 2000/07/15 14:52:43 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getdtables.c,v 1.1.1.1 2000/07/15 14:52:43 nick Exp $";
#endif

/* unix.c.getdtables. Implementation of getdtablesize ().

   Written by Nick Burrett, 13 October 1996.  */

#include <limits.h>
#include <unistd.h>

/* Return the maximum number of file descriptors
   the current process could possibly have.  */

/* A stub function that really should take into account the
   actual number of files open on RISC OS and determine whether
   that OPEN_MAX files can still be opened. Also should take
   into account getrusage ().  */

int
getdtablesize (void)
{
  return OPEN_MAX;
}
