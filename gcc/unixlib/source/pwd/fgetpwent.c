/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id$";
#endif

/* pwd.c.fgetpwent. Read a password file entry.

   Written by Nick Burrett, 13 October 1996.  */

#include <stddef.h>
#include <stdio.h>
#include <pwd.h>

/* Read one entry from the given stream.
   Not thread safe */
struct passwd *
fgetpwent (FILE * stream)
{
  static struct passwd pwd;
  static char buffer[256];

  if (stream == NULL)
    return NULL;

  return __pwdread (stream, &pwd, buffer, sizeof (buffer));
}
