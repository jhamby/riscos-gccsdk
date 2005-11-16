/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pwd/getpwuid.c,v $
 * $Date: 2005/04/14 12:13:09 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* pwd.c.getpwuid. Search for an entry with a matching user ID.

   This is a POSIX.1 function written by Nick Burrett, 13 October 1996.  */

#include <stddef.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include <pthread.h>

/* Search for an entry with a matching uid.
   Defined by POSIX as not threadsafe.  */
struct passwd *
getpwuid (uid_t uid)
{
  FILE *stream;
  struct passwd *p;

  PTHREAD_UNSAFE_CANCELLATION

  stream = fopen ("/etc/passwd", "r");
  if (stream == NULL)
    return __pwddefault ();

  while ((p = fgetpwent (stream)) != NULL)
    if (p->pw_uid == uid)
      break;

  fclose (stream);
  return p;
}
