/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/pwd/c/getpwent,v $
 * $Date: 1998/01/29 21:15:16 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getpwent,v 1.6 1998/01/29 21:15:16 unixlib Exp $";
#endif

/* Password-file operations.
   Contributed by Nick Burrett, 13 October 1996.  */

#include <stddef.h>
#include <stdio.h>
#include <pwd.h>

static FILE *stream = NULL;

/* Rewind the stream.  */
void
setpwent (void)
{
  if (stream != NULL)
    rewind (stream);
}

/* Close the stream.  */
void
endpwent (void)
{
  if (stream != NULL)
    {
      fclose (stream);
      stream = NULL;
    }
}

/* Return one entry from the password file.  */
struct passwd *
getpwent (void)
{
  static struct passwd pwd;

  /* Open the password file if not already open.  */
  if (stream == NULL)
    stream = fopen ("/etc/passwd", "r");

  if (stream == NULL)
    return NULL;

  return __pwdread (stream, &pwd);
}

/* Return one entry from the password file (re-entrant version).  */
int
getpwent_r (struct passwd *result_buf, char *buffer, size_t buflen,
	    struct passwd **result)
{
  return fgetpwent_r (stream, result_buf, buffer, buflen, result);
}
