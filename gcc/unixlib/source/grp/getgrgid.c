/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/grp/Attic/getgrgid.c,v $
 * $Date: 2001/09/11 13:32:33 $
 * $Revision: 1.1.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getgrgid.c,v 1.1.2.1 2001/09/11 13:32:33 admin Exp $";
#endif

/* Search for an entry with a matching user ID. */

#include <stddef.h>
#include <stdio.h>
#include <grp.h>
#include <sys/types.h>

/* Search for an entry with a matching gid.  */
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
