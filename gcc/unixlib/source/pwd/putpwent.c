/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/pwd/c/putpwent,v $
 * $Date: 1997/10/09 20:00:25 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: putpwent,v 1.5 1997/10/09 20:00:25 unixlib Exp $";
#endif

/* pwd.c.putpwent. Write an entry to stream.

   System V compatibility function.
   Written by Nick Burrett, 13 October 1996.  */

#include <errno.h>
#include <stdio.h>
#include <pwd.h>

/* Write an entry to the given stream.
   This must know the format of the password file.  */
int
putpwent (const struct passwd *p, FILE * stream)
{
  if (p == NULL || stream == NULL)
    {
      errno = EINVAL;
      return -1;
    }

  if (fprintf (stream, "%s:%s:%u:%u:%s:%s:%s\n",
	       p->pw_name, p->pw_passwd,
	       p->pw_uid, p->pw_gid,
	       p->pw_gecos, p->pw_dir, p->pw_shell) < 0)
    return -1;

  return 0;
}
