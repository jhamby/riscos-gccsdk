/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pwd/putpwent.c,v $
 * $Date: 2005/04/14 12:13:09 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

/* pwd.c.putpwent. Write an entry to stream.

   System V compatibility function.
   Written by Nick Burrett, 13 October 1996.  */

#include <errno.h>
#include <stdio.h>
#include <pwd.h>
#include <pthread.h>

/* Write an entry to the given stream.
   This must know the format of the password file.  */
int
putpwent (const struct passwd *p, FILE * stream)
{
  PTHREAD_UNSAFE_CANCELLATION
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
