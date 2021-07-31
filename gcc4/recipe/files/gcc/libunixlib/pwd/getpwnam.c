/* getpwnam ()
 * Search for an entry with a matching username (POSIX.1 function)
 * written by Nick Burrett, 13 October 1996.
 * Copyright (c) 1996-2008 UnixLib Developers
 */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>

#include <pthread.h>
#include <internal/unix.h>

/* Search for an entry with a matching name.
   Defined by POSIX as not threadsafe.  */
struct passwd *
getpwnam (const char *name)
{
  FILE *stream;
  struct passwd *p;

  PTHREAD_UNSAFE_CANCELLATION

  stream = fopen ("/etc/passwd", "r");
  if (stream == NULL)
    return __pwddefault ();

  while ((p = fgetpwent (stream)) != NULL)
    if (!strcmp (p->pw_name, name))
      break;

  fclose (stream);
  return p;
}
