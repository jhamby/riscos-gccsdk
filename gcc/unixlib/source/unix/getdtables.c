/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/unix/c/getdtables,v $
 * $Date: 1997/10/09 20:00:50 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getdtables,v 1.4 1997/10/09 20:00:50 unixlib Exp $";
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
