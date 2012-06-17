/* getpwuid_r ()
 * Search for an entry with a matching user ID (re-entrant version).
 * Written by Nick Burrett, 10 December 1997.
 * Copyright (c) 1997-2012 UnixLib Developers
 */

#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <sys/types.h>

#include <pthread.h>
#include <internal/unix.h>
#include <string.h>

static int copy_default (struct passwd *to,
			 char *buffer,
			 size_t buflen,
			 struct passwd *from)
{
  char *buffp = buffer;
  size_t buffer_remaining = buflen;
  size_t member_len;

#define COPY_MEMBER(member) \
  if (from->member == NULL) \
    to->member = NULL; \
  else \
  { \
    member_len = strlen (from->member) + 1; \
    if (member_len > buffer_remaining) \
      return __set_errno (ERANGE); \
    memcpy (buffp, from->member, member_len); \
    to->member = buffp; \
    buffp += member_len; \
    buffer_remaining -= member_len; \
  }

  to->pw_uid = from->pw_uid;
  to->pw_gid = from->pw_gid;
  COPY_MEMBER (pw_name)
  COPY_MEMBER (pw_passwd)
  COPY_MEMBER (pw_gecos)
  COPY_MEMBER (pw_dir)
  COPY_MEMBER (pw_shell)

  return 0;
}

int
getpwuid_r (uid_t uid, struct passwd *resbuf, char *buffer, size_t buflen,
	    struct passwd **result)
{
  PTHREAD_SAFE_CANCELLATION

  if (resbuf == NULL)
    return __set_errno (EINVAL);

  FILE *stream = fopen ("/etc/passwd", "r");
  if (stream == NULL)
    {
      /* FIXME: Should result point to the user buffer as below?
                Can result be NULL? (It's tested for below, but not here) */
      *result = __pwddefault ();

      /* Fill the user supplied buffer with the default values.  */
      return copy_default (resbuf, buffer, buflen, *result);
    }

  struct passwd *p;
  while ((p = __pwdread (stream, resbuf, buffer, buflen)) != NULL)
    {
      if (resbuf->pw_uid == uid)
        break;
    }

  fclose (stream);

  if (p == NULL)
    return -1;

  if (result != NULL)
    *result = resbuf;

  return 0;
}
