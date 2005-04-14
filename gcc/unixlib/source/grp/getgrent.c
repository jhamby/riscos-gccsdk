/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/grp/getgrent.c,v $
 * $Date: 2003/01/21 17:54:22 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getgrent.c,v 1.3 2003/01/21 17:54:22 admin Exp $";
#endif

/* Group Password-file operations. */

#include <stddef.h>
#include <stdio.h>
#include <grp.h>
#include <pthread.h>

static FILE *stream = NULL;

/* Rewind the stream.
   Defined by POSIX as not threadsafe */
void
setgrent (void)
{
  PTHREAD_UNSAFE_CANCELLATION
  if (stream != NULL)
    rewind (stream);
}

/* Close the stream.
   Defined by POSIX as not threadsafe */
void
endgrent (void)
{
  PTHREAD_UNSAFE_CANCELLATION
  if (stream != NULL)
    {
      fclose (stream);
      stream = NULL;
    }
}

/* Return one entry from the password file.
   Defined by POSIX as not threadsafe */
struct group *
getgrent (void)
{
  static struct group grp;
  static char buffer[256];

  PTHREAD_UNSAFE_CANCELLATION

  /* Open the password file if not already open.  */
  if (stream == NULL)
    stream = fopen ("/etc/group", "r");

  if (stream == NULL)
    return NULL;

  return __grpread (stream, &grp, buffer, sizeof (buffer));
}

/* Return one entry from the password file (re-entrant version).  */
int
getgrent_r (struct group *result_buf, char *buffer, size_t buflen,
	    struct group **result)
{
  PTHREAD_SAFE_CANCELLATION
  return fgetgrent_r (stream, result_buf, buffer, buflen, result);
}
