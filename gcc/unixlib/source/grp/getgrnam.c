/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/grp/getgrnam.c,v $
 * $Date: 2005/04/14 12:13:09 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

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
