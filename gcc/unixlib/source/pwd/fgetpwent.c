/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pwd/fgetpwent.c,v $
 * $Date: 2000/07/15 14:52:27 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fgetpwent.c,v 1.1.1.1 2000/07/15 14:52:27 nick Exp $";
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
