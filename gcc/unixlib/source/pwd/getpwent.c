/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pwd/getpwent.c,v $
 * $Date: 2000/07/15 14:52:27 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getpwent.c,v 1.1.1.1 2000/07/15 14:52:27 nick Exp $";
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
