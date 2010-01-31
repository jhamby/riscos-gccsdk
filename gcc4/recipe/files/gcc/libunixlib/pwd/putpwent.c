/* putpwent ()
 * Write an entry to stream.
 * System V compatibility function.
 * Written by Nick Burrett, 13 October 1996.
 * Copyright (c) 1996-2008 UnixLib Developers
 */

#include <errno.h>
#include <stdio.h>
#include <pwd.h>

#include <pthread.h>
#include <internal/unix.h>

/* Write an entry to the given stream.
   This must know the format of the password file.  */
int
putpwent (const struct passwd *p, FILE * stream)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (p == NULL || stream == NULL)
    return __set_errno (EINVAL);

  if (fprintf (stream, "%s:%s:%u:%u:%s:%s:%s\n",
	       p->pw_name, p->pw_passwd,
	       p->pw_uid, p->pw_gid,
	       p->pw_gecos, p->pw_dir, p->pw_shell) < 0)
    return -1;

  return 0;
}
