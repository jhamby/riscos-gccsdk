/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pwd/fgetpwent.c,v $
 * $Date: 2005/04/14 12:13:09 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

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
