/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pwd/getpwnam.c,v $
 * $Date: 2003/01/21 17:54:22 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getpwnam.c,v 1.4 2003/01/21 17:54:22 admin Exp $";
#endif

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
