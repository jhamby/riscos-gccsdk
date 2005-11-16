/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pwd/getpwent.c,v $
 * $Date: 2005/04/14 12:13:09 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* Password-file operations.
   Contributed by Nick Burrett, 13 October 1996.  */

#include <stddef.h>
#include <stdio.h>
#include <pwd.h>
#include <pthread.h>

static FILE *stream = NULL;

/* Rewind the stream.
   Defined by POSIX as not threadsafe.  */
void
setpwent (void)
{
  PTHREAD_UNSAFE_CANCELLATION
  if (stream != NULL)
    rewind (stream);
}

/* Close the stream.
   Defined by POSIX as not threadsafe.  */
void
endpwent (void)
{
  PTHREAD_UNSAFE_CANCELLATION
  if (stream != NULL)
    {
      fclose (stream);
      stream = NULL;
    }
}

/* Return one entry from the password file.
   Defined by POSIX as not threadsafe.  */
struct passwd *
getpwent (void)
{
  static struct passwd pwd;
  static char buffer[256];

  PTHREAD_UNSAFE_CANCELLATION

  /* Open the password file if not already open.  */
  if (stream == NULL)
    stream = fopen ("/etc/passwd", "r");

  if (stream == NULL)
    return NULL;

  return __pwdread (stream, &pwd, buffer, sizeof (buffer));
}

/* Return one entry from the password file (re-entrant version).  */
int
getpwent_r (struct passwd *result_buf, char *buffer, size_t buflen,
	    struct passwd **result)
{
  PTHREAD_SAFE_CANCELLATION
  return fgetpwent_r (stream, result_buf, buffer, buflen, result);
}
