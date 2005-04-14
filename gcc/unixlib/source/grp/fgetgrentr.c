/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/grp/fgetgrentr.c,v $
 * $Date: 2003/01/21 17:54:22 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fgetgrentr.c,v 1.3 2003/01/21 17:54:22 admin Exp $";
#endif

/* Read a group password file entry (re-entrant version). */

#include <stddef.h>
#include <stdio.h>
#include <grp.h>
#include <pthread.h>

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
