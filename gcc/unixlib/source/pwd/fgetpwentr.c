/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pwd/fgetpwentr.c,v $
 * $Date: 2005/04/14 12:13:09 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

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
