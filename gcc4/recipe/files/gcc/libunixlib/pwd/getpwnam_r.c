/* getpwnam_r ()
 * Search for an entry with a matching username (re-entrant version).
 * Written by Nick Burrett, 10 December 1997.
 * Copyright (c) 1997-2010 UnixLib Developers
 */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <errno.h>

#include <pthread.h>
#include <internal/unix.h>

/* Search for an entry with a matching name.  */
int
getpwnam_r (const char *name, struct passwd *result_buf, char *buffer,
            size_t buflen, struct passwd **result)
{
  PTHREAD_SAFE_CANCELLATION

  if (result_buf == NULL || buffer == NULL)
    return __set_errno (EINVAL);

  FILE *stream = fopen ("/etc/passwd", "r");

  struct passwd *p;
  if (stream == NULL)
    {
      p = __pwddefault ();

      if (result_buf)
	memcpy (result_buf, p, sizeof(struct passwd));
    }
  else
    {
      while ((p = __pwdread (stream, result_buf, buffer, buflen)) != NULL)
        {
          if (strcmp (result_buf->pw_name, name) == 0)
            break;
        }

      fclose (stream);
   }

  if (p == NULL)
    return -1;
  if (result != NULL)
    *result = result_buf;
  strncpy (buffer, result ? (*result)->pw_passwd : p->pw_passwd, buflen);

  return 0;
}
