/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/grp/Attic/getgrnam_r.c,v $
 * $Date: 2001/09/11 13:32:33 $
 * $Revision: 1.1.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getgrnam_r.c,v 1.1.2.1 2001/09/11 13:32:33 admin Exp $";
#endif

/* Search for an entry with a matching group name (re-entrant version). */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <grp.h>
#include <errno.h>

/* Search for an entry with a matching name.  */
int
getgrnam_r (const char *name, struct group *result_buf, char *buffer,
	    size_t buflen, struct group **result)
{
  FILE *stream;
  struct group *grp;

  if (result_buf == NULL)
    return __set_errno (EINVAL);

  stream = fopen ("/etc/group", "r");
  if (stream == NULL)
    return -1;

  grp = __grpread (stream, result_buf);
  while (grp != NULL)
    {
      if (strcmp (result_buf->gr_name, name) == 0)
        break;
      grp = __grpread (stream, result_buf);
    }

  fclose (stream);

  if (grp == NULL)
    return -1;

  if (result != NULL)
    *result = result_buf;

  return 0;
}
