/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/grp/getgrgid.c,v $
 * $Date: 2005/04/14 12:13:09 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

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
