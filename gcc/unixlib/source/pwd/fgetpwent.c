/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/pwd/c/fgetpwent,v $
 * $Date: 1997/10/09 20:00:24 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fgetpwent,v 1.5 1997/10/09 20:00:24 unixlib Exp $";
#endif

/* pwd.c.fgetpwent. Read a password file entry.

   Written by Nick Burrett, 13 October 1996.  */

#include <stddef.h>
#include <stdio.h>
#include <pwd.h>

/* Read one entry from the given stream.  */
struct passwd *
fgetpwent (FILE * stream)
{
  static struct passwd pwd;

  if (stream == NULL)
    return NULL;

  return __pwdread (stream, &pwd);
}
