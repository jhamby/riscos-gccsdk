/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/pwd/c/getlogin,v $
 * $Date: 1998/01/29 21:15:15 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getlogin,v 1.5 1998/01/29 21:15:15 unixlib Exp $";
#endif

#include <pwd.h>
#include <string.h>
#include <unistd.h>

/* Return a pointer to a string containing the name of the user logged
   in on the controlling terminal of the process.  */

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
