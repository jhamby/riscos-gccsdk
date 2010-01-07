/* getpwnam_r ()
 * Search for an entry with a matching username (re-entrant version).
 * Written by Nick Burrett, 10 December 1997.
 * Copyright (c) 1997-2008 UnixLib Developers
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
  FILE *stream;
  struct passwd *p;

  PTHREAD_SAFE_CANCELLATION

  if (result_buf == NULL || buffer == NULL)
    return __set_errno (EINVAL);

  stream = fopen ("/etc/passwd", "r");

  if (stream == NULL)
    {
      p = __pwddefault ();

      if (result_buf) memcpy (result_buf, p, sizeof(struct passwd));
    }
  else
    {
      p = __pwdread (stream, result_buf, buffer, buflen);
      while (p != NULL)
        {
          if (strcmp (result_buf->pw_name, name) == 0)
            break;
          p = __pwdread (stream, result_buf, buffer, buflen);
        }

      fclose (stream);
   }

  if (p == NULL) return -1;
  if (result != NULL) *result = result_buf;
  if (buffer != NULL) strncpy (buffer, result ? (*result)->pw_passwd : p->pw_passwd, buflen);

  return 0;
}
