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
    {
      *result = __pwddefault ();
      return 0;
    }

  p = __pwdread (stream, resbuf, buffer, buflen);
  while (p != NULL)
    {
      if (resbuf->pw_uid == uid)
        break;
      p = __pwdread (stream, resbuf, buffer, buflen);
    }

  fclose (stream);

  if (p == NULL)
    return -1;

  if (result != NULL)
    *result = resbuf;

  return 0;
}
