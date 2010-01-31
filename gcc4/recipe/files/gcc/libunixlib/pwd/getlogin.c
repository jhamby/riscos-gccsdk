/* getlogin ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <pwd.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
#include <internal/unix.h>

/* Return a pointer to a string containing the name of the user logged
   in on the controlling terminal of the process.
   Defined by POSIX as not threadsafe  */

char *
getlogin (void)
{
  static char name[L_cuserid];
  char buf[256 /* NSS_BUFLEN_PASSWD */];
  struct passwd pwent;
  struct passwd *pwptr;

  PTHREAD_UNSAFE_CANCELLATION

  if (getpwuid_r (geteuid (), &pwent, buf, sizeof (buf), &pwptr))
    return NULL;

  strncpy (name, pwptr->pw_name, L_cuserid);
  
  return name;
}
