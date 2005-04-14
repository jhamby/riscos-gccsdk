/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/grp/getgrgid_r.c,v $
 * $Date: 2003/01/21 17:54:22 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getgrgid_r.c,v 1.3 2003/01/21 17:54:22 admin Exp $";
#endif

/* Search for an entry with a matching group ID (re-entrant version). */

#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <grp.h>
#include <sys/types.h>
#include <pthread.h>

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
