/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pwd/getpw.c,v $
 * $Date: 2001/01/29 15:10:20 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getpw.c,v 1.2 2001/01/29 15:10:20 admin Exp $";
#endif

#include <errno.h>
#include <stdio.h>
#include <pwd.h>
#include <pthread.h>

/* Re-construct the password-file line for the given uid
   in the given buffer.  This knows the format that the caller
   will expect, but this need not be the format of the password file.  */
int getpw (uid_t uid, char *buf)
{
  struct passwd *p;

  PTHREAD_UNSAFE_CANCELLATION

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
