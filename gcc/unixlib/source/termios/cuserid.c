/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/cuserid.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: cuserid.c,v 1.2 2001/01/29 15:10:21 admin Exp $";
#endif

#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

/* Return the username of the caller.
   If S is not NULL, it points to a buffer of at least L_cuserid bytes
   into which the name is copied; otherwise, a static buffer is used.  */

/* Not threadsafe. Defined by POSIX as LEGACY */
char *
cuserid (char *s)
{
  static char name[L_cuserid];
  char buf[255 /* NSS_BUFLEN_PASSWD */];
  struct passwd pwent;
  struct passwd *pwptr;

  if (getpwuid_r (geteuid (), &pwent, buf, sizeof (buf), &pwptr))
    {
      if (s != NULL)
	s[0] = '\0';
      return NULL;
    }

  if (s == NULL)
    s = name;
  return strncpy (s, pwptr->pw_name, L_cuserid);
}
