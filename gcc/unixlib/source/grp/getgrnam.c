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

/* Search for an entry with a matching username. */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <grp.h>

/* Search for an entry with a matching name.
   Defined by POSIX as not threadsafe */
struct group *
getgrnam (const char *name)
{
  FILE *stream;
  struct group *grp;

  stream = fopen ("/etc/group", "r");
  if (stream == NULL)
    return NULL;

  while ((grp = fgetgrent (stream)) != NULL)
    if (!strcmp (grp->gr_name, name))
      break;

  fclose (stream);
  return grp;
}
