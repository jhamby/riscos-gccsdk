/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/pwd/c/fgetpwentr,v $
 * $Date: 1998/01/29 21:15:15 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fgetpwentr,v 1.1 1998/01/29 21:15:15 unixlib Exp $";
#endif

/* Read a password file entry (re-entrant version).
   Written by Nick Burrett, 10 December 1997.  */

#include <stddef.h>
#include <stdio.h>
#include <pwd.h>

/* Read one entry from the given stream.  
   We currently do not place anything in buffer.  */
int
fgetpwent_r (FILE *stream, struct passwd *result_buf, char *buffer,
	     size_t buflen, struct passwd **result)
{
  struct passwd *p;

  if (stream == NULL)
    return -1;

  p = __pwdread (stream, result_buf);
  if (p == NULL)
    return -1;

  if (result != NULL)
    *result = result_buf;

  return 0;
}
