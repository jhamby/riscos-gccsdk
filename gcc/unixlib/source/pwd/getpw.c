/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/pwd/c/getpw,v $
 * $Date: 1997/10/09 20:00:25 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getpw,v 1.5 1997/10/09 20:00:25 unixlib Exp $";
#endif

#include <errno.h>
#include <stdio.h>
#include <pwd.h>


/* Re-construct the password-file line for the given uid
   in the given buffer.  This knows the format that the caller
   will expect, but this need not be the format of the password file.  */
int
getpw (__uid_t uid, char *buf)
{
  register struct passwd *p;

  if (buf == NULL)
    {
      errno = EINVAL;
      return -1;
    }

  p = getpwuid (uid);
  if (p == NULL)
    return -1;

  if (sprintf (buf, "%s:%s:%u:%u:%s:%s:%s", p->pw_name, p->pw_passwd,
	     p->pw_uid, p->pw_gid, p->pw_gecos, p->pw_dir, p->pw_shell) < 0)
    return -1;

  return 0;
}
