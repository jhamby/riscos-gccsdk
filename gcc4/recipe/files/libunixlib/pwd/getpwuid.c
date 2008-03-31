/* getpwuid ()
 * Search for an entry with a matching user ID (POSIX.1 function)
 * Written by Nick Burrett, 13 October 1996.
 * Copyright (c) 1996-2008 UnixLib Developers
 */

#include <stddef.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>

#include <pthread.h>
#include <internal/unix.h>

/* Search for an entry with a matching uid.
   Defined by POSIX as not threadsafe.  */
struct passwd *
getpwuid (uid_t uid)
{
  FILE *stream;
  struct passwd *p;

  PTHREAD_UNSAFE_CANCELLATION

  stream = fopen ("/etc/passwd", "r");
  if (stream == NULL)
    return __pwddefault ();

  while ((p = fgetpwent (stream)) != NULL)
    if (p->pw_uid == uid)
      break;

  fclose (stream);
  return p;
}
