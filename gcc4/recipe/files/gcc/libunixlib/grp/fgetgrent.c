/* fgetgrent ()
 * Read a group password file entry.
 *
 * Copyright (c) 2002-2008 UnixLib Developers
 */

#include <stddef.h>
#include <stdio.h>
#include <grp.h>

#include <pthread.h>
#include <internal/unix.h>

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
