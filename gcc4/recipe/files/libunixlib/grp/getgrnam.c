/* getgrnam ()
 * Search for an entry with a matching username.
 *
 * Copyright (c) 2000-2006 UnixLib Developers
 */

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
