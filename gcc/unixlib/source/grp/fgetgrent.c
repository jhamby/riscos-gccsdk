/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/grp/fgetgrent.c,v $
 * $Date: 2003/01/21 17:54:22 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fgetgrent.c,v 1.3 2003/01/21 17:54:22 admin Exp $";
#endif

/* Read a group password file entry. */

#include <stddef.h>
#include <stdio.h>
#include <grp.h>
#include <pthread.h>

/* Read one entry from the given stream.
   Not thread safe.  */
struct group *
fgetgrent (FILE * stream)
{
  static struct group grp;
  static char buffer[256];

  PTHREAD_UNSAFE_CANCELLATION

  if (stream == NULL)
    return NULL;

  return __grpread (stream, &grp, buffer, sizeof (buffer));
}
