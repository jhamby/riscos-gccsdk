/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id$";
#endif

#include <pwd.h>
#include <string.h>
#include <unistd.h>

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

  if (getpwuid_r (geteuid (), &pwent, buf, sizeof (buf), &pwptr))
    return NULL;

  strncpy (name, pwptr->pw_name, L_cuserid);
  
  return name;
}
