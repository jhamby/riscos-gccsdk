/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pwd/fgetpwent.c,v $
 * $Date: 2003/01/21 17:54:22 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: fgetpwent.c,v 1.3 2003/01/21 17:54:22 admin Exp $";
#endif

/* pwd.c.fgetpwent. Read a password file entry.

   Written by Nick Burrett, 13 October 1996.  */

#include <stddef.h>
#include <stdio.h>
#include <pwd.h>
#include <pthread.h>

/* Read one entry from the given stream.
   Not thread safe.  */
struct passwd *
fgetpwent (FILE * stream)
{
  static struct passwd pwd;
  static char buffer[256];

  PTHREAD_UNSAFE_CANCELLATION

  if (stream == NULL)
    return NULL;

  return __pwdread (stream, &pwd, buffer, sizeof (buffer));
}
