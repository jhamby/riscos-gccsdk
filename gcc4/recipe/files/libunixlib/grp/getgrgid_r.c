/* getgrgid_r ()
 * Search for an entry with a matching group ID (re-entrant version).
 *
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <grp.h>
#include <sys/types.h>

#include <pthread.h>
#include <internal/unix.h>

int
getgrgid_r (gid_t gid, struct group *resbuf, char *buffer, size_t buflen,
	    struct group **result)
{
  FILE *stream;
  struct group *grp;

  PTHREAD_SAFE_CANCELLATION

  if (resbuf == NULL || buffer == NULL)
    return __set_errno (EINVAL);

  stream = fopen ("/etc/group", "r");
  if (stream == NULL)
    return -1;

  grp = __grpread (stream, resbuf, buffer, buflen);
  while (grp != NULL)
    {
      if (resbuf->gr_gid == gid)
        break;
      grp = __grpread (stream, resbuf, buffer, buflen);
    }

  fclose (stream);

  if (grp == NULL)
    return -1;

  if (result != NULL)
    *result = resbuf;

  return 0;
}
