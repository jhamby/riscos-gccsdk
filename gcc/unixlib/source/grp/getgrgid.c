/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/grp/getgrgid.c,v $
 * $Date: 2003/01/21 17:54:22 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getgrgid.c,v 1.3 2003/01/21 17:54:22 admin Exp $";
#endif

/* Search for an entry with a matching user ID. */

#include <stddef.h>
#include <stdio.h>
#include <grp.h>
#include <sys/types.h>
#include <pthread.h>

/* Search for an entry with a matching gid.
   Defined by POSIX as not threadsafe.  */
struct group *
getgrgid (gid_t gid)
{
  FILE *stream;
  struct group *grp;

  PTHREAD_UNSAFE_CANCELLATION

  stream = fopen ("/etc/group", "r");
  if (stream == NULL)
    return NULL;

  while ((grp = fgetgrent (stream)) != NULL)
    if (grp->gr_gid == gid)
      break;

  fclose (stream);
  return grp;
}
