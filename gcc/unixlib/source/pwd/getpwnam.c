/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/pwd/c/getpwnam,v $
 * $Date: 1997/10/09 20:00:25 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getpwnam,v 1.5 1997/10/09 20:00:25 unixlib Exp $";
#endif

/* pwd.c.getpwnam. Search for an entry with a matching username.

   This is a POSIX.1 function written by Nick Burrett, 13 October 1996.  */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>

/* Search for an entry with a matching name.  */
struct passwd *
getpwnam (const char *name)
{
  FILE *stream;
  struct passwd *p;

  stream = fopen ("/etc/passwd", "r");
  if (stream == NULL)
    return NULL;

  while ((p = fgetpwent (stream)) != NULL)
    if (!strcmp (p->pw_name, name))
      break;

  fclose (stream);
  return p;
}
