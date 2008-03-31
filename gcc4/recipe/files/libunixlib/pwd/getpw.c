/* getpw ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <errno.h>
#include <stdio.h>
#include <pwd.h>

#include <pthread.h>
#include <internal/unix.h>

/* Re-construct the password-file line for the given uid
   in the given buffer.  This knows the format that the caller
   will expect, but this need not be the format of the password file.  */
int getpw (uid_t uid, char *buf)
{
  struct passwd *p;

  PTHREAD_UNSAFE_CANCELLATION

  if (buf == NULL)
    return __set_errno (EINVAL);

  p = getpwuid (uid);
  if (p == NULL)
    return -1;

  if (sprintf (buf, "%s:%s:%u:%u:%s:%s:%s", p->pw_name, p->pw_passwd,
	     p->pw_uid, p->pw_gid, p->pw_gecos, p->pw_dir, p->pw_shell) < 0)
    return -1;

  return 0;
}
