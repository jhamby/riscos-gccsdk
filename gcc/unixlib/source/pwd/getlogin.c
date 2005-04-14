/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pwd/getlogin.c,v $
 * $Date: 2003/01/21 17:54:22 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getlogin.c,v 1.3 2003/01/21 17:54:22 admin Exp $";
#endif

#include <pwd.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

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
