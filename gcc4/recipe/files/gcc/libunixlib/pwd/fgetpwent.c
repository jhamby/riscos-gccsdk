/* fgetpwent ()
 * Read a password file entry.
 * Written by Nick Burrett, 13 October 1996.
 * Copyright (c) 1996-2008 UnixLib Developers
 */

#include <stddef.h>
#include <stdio.h>
#include <pwd.h>

#include <pthread.h>
#include <internal/unix.h>

/* Read one entry from the given stream.
   Not thread safe.  */
struct passwd *
fgetpwent (FILE * stream)
{
  static struct passwd pwd;
  static char buffer[256];

  PTHREAD_UNSAFE_CANCELLATION

  if (stream == NULL)
    return NULL;

  return __pwdread (stream, &pwd, buffer, sizeof (buffer));
}
