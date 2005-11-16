/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pwd/getpwnam.c,v $
 * $Date: 2005/04/14 12:13:09 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* pwd.c.getpwnam. Search for an entry with a matching username.

   This is a POSIX.1 function written by Nick Burrett, 13 October 1996.  */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <pthread.h>

/* Search for an entry with a matching name.
   Defined by POSIX as not threadsafe.  */
struct passwd *
getpwnam (const char *name)
{
  FILE *stream;
  struct passwd *p;

  PTHREAD_UNSAFE_CANCELLATION

  stream = fopen ("/etc/passwd", "r");
  if (stream == NULL)
    return __pwddefault ();

  while ((p = fgetpwent (stream)) != NULL)
    if (!strcmp (p->pw_name, name))
      break;

  fclose (stream);
  return p;
}
