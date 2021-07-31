/* fgetgrent_r ()
 * Read a group password file entry (re-entrant version).
 *
 * Copyright (c) 2002-2008 UnixLib Developers
 */

#include <stddef.h>
#include <stdio.h>
#include <grp.h>

#include <pthread.h>
#include <internal/unix.h>

/* Read one entry from the given stream.  */
int
fgetgrent_r (FILE *stream, struct group *result_buf, char *buffer,
	     size_t buflen, struct group **result)
{
  struct group *grp;

  PTHREAD_SAFE_CANCELLATION

  if (stream == NULL || buffer == NULL)
    return -1;

  grp = __grpread (stream, result_buf, buffer, buflen);
  if (grp == NULL)
    return -1;

  if (result != NULL)
    *result = result_buf;

  return 0;
}
