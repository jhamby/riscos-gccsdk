/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/cuserid.c,v $
 * $Date: 2000/07/15 14:52:35 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: cuserid.c,v 1.1.1.1 2000/07/15 14:52:35 nick Exp $";
#endif

#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

/* Return the username of the caller.
   If S is not NULL, it points to a buffer of at least L_cuserid bytes
   into which the name is copied; otherwise, a static buffer is used.  */
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
