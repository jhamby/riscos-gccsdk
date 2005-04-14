/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/grp/getgrnam.c,v $
 * $Date: 2003/01/21 17:54:22 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getgrnam.c,v 1.3 2003/01/21 17:54:22 admin Exp $";
#endif

/* Search for an entry with a matching username. */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <grp.h>
#include <pthread.h>

/* Search for an entry with a matching name.
   Defined by POSIX as not threadsafe.  */
struct group *
getgrnam (const char *name)
{
  FILE *stream;
  struct group *grp;

  PTHREAD_UNSAFE_CANCELLATION

  stream = fopen ("/etc/group", "r");
  if (stream == NULL)
    return NULL;

  while ((grp = fgetgrent (stream)) != NULL)
    if (!strcmp (grp->gr_name, name))
      break;

  fclose (stream);
  return grp;
}
