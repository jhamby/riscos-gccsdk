/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/pwd/c/getpwuid,v $
 * $Date: 1997/10/09 20:00:25 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getpwuid,v 1.5 1997/10/09 20:00:25 unixlib Exp $";
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
    return NULL;

  while ((p = fgetpwent (stream)) != NULL)
    if (p->pw_uid == uid)
      break;

  fclose (stream);
  return p;
}
