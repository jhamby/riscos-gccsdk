/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pwd/getpwuid.c,v $
 * $Date: 2001/01/29 15:10:20 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getpwuid.c,v 1.2 2001/01/29 15:10:20 admin Exp $";
#endif

/* pwd.c.getpwuid. Search for an entry with a matching user ID.

   This is a POSIX.1 function written by Nick Burrett, 13 October 1996.  */

#include <stddef.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>

/* Search for an entry with a matching uid.  */
struct passwd *
getpwuid (uid_t uid)
{
  FILE *stream;
  struct passwd *p;

  stream = fopen ("/etc/passwd", "r");
  if (stream == NULL)
    return __pwddefault ();

  while ((p = fgetpwent (stream)) != NULL)
    if (p->pw_uid == uid)
      break;

  fclose (stream);
  return p;
}
