/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id$";
#endif

/* Search for an entry with a matching user ID. */

#include <stddef.h>
#include <stdio.h>
#include <grp.h>
#include <sys/types.h>

/* Search for an entry with a matching gid.
   Defined by POSIX as not threadsafe */
struct group *
getgrgid (gid_t gid)
{
  FILE *stream;
  struct group *grp;

  stream = fopen ("/etc/group", "r");
  if (stream == NULL)
    return NULL;

  while ((grp = fgetgrent (stream)) != NULL)
    if (grp->gr_gid == gid)
      break;

  fclose (stream);
  return grp;
}
