/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/grp/Attic/getgrgid_r.c,v $
 * $Date: 2001/09/11 13:32:33 $
 * $Revision: 1.1.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getgrgid_r.c,v 1.1.2.1 2001/09/11 13:32:33 admin Exp $";
#endif

/* Search for an entry with a matching group ID (re-entrant version). */

#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <grp.h>
#include <sys/types.h>

int
getgrgid_r (gid_t gid, struct group *resbuf, char *buffer, size_t buflen,
	    struct group **result)
{
  FILE *stream;
  struct group *grp;

  if (resbuf == NULL)
    return __set_errno (EINVAL);

  stream = fopen ("/etc/group", "r");
  if (stream == NULL)
    return -1;

  grp = __grpread (stream, resbuf);
  while (grp != NULL)
    {
      if (resbuf->gr_gid == gid)
        break;
      grp = __grpread (stream, resbuf);
    }

  fclose (stream);

  if (grp == NULL)
    return -1;

  if (result != NULL)
    *result = resbuf;

  return 0;
}
