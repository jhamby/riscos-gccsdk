/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pwd/fgetpwentr.c,v $
 * $Date: 2003/01/21 17:54:22 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fgetpwentr.c,v 1.3 2003/01/21 17:54:22 admin Exp $";
#endif

/* Read a password file entry (re-entrant version).
   Written by Nick Burrett, 10 December 1997.  */

#include <stddef.h>
#include <stdio.h>
#include <pwd.h>
#include <pthread.h>

/* Read one entry from the given stream.  */
int
fgetpwent_r (FILE *stream, struct passwd *result_buf, char *buffer,
	     size_t buflen, struct passwd **result)
{
  struct passwd *p;

  PTHREAD_SAFE_CANCELLATION

  if (stream == NULL)
    return -1;

  p = __pwdread (stream, result_buf, buffer, buflen);
  if (p == NULL)
    return -1;

  if (result != NULL)
    *result = result_buf;

  return 0;
}
