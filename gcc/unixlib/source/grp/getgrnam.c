/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/grp/Attic/getgrnam.c,v $
 * $Date: 2001/09/11 13:32:33 $
 * $Revision: 1.1.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getgrnam.c,v 1.1.2.1 2001/09/11 13:32:33 admin Exp $";
#endif

/* Search for an entry with a matching username. */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <grp.h>

/* Search for an entry with a matching name.  */
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
