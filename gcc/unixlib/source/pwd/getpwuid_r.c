/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/pwd/c/getpwuid_r,v $
 * $Date: 1998/01/29 21:15:16 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: getpwuid_r,v 1.1 1998/01/29 21:15:16 unixlib Exp $";
#endif

/* Search for an entry with a matching user ID (re-entrant version).
   Contributed by Nick Burrett, 10 December 1997.  */

#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <sys/types.h>

int
getpwuid_r (uid_t uid, struct passwd *resbuf, char *buffer, size_t buflen,
	    struct passwd **result)
{
  FILE *stream;
  struct passwd *p;

  if (resbuf == NULL)
    return __set_errno (EINVAL);

  stream = fopen ("/etc/passwd", "r");
  if (stream == NULL)
    return -1;

  p = __pwdread (stream, resbuf);
  while (p != NULL)
    {
      if (resbuf->pw_uid == uid)
        break;
      p = __pwdread (stream, resbuf);
    }

  fclose (stream);

  if (p == NULL)
    return -1;

  if (result != NULL)
    *result = resbuf;

  return 0;
}
