/* setgrent (), endgrent (), getgrent (), getgrent_r ()
 * Group Password-file operations.
 *
 * Copyright (c) 2002-2008 UnixLib Developers
 */


#include <stddef.h>
#include <stdio.h>
#include <grp.h>

#include <pthread.h>
#include <internal/unix.h>

static FILE *stream = NULL;

/* Rewind the stream.
   Defined by POSIX as not threadsafe */
void
setgrent (void)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (stream != NULL)
    rewind (stream);
}

/* Close the stream.
   Defined by POSIX as not threadsafe */
void
endgrent (void)
{
  PTHREAD_UNSAFE_CANCELLATION

  if (stream != NULL)
    {
      fclose (stream);
      stream = NULL;
    }
}

/* Return one entry from the password file.
   Defined by POSIX as not threadsafe */
struct group *
getgrent (void)
{
  static struct group grp;
  static char buffer[256];

  PTHREAD_UNSAFE_CANCELLATION

  /* Open the password file if not already open.  */
  if (stream == NULL)
    stream = fopen ("/etc/group", "r");

  if (stream == NULL)
    return NULL;

  return __grpread (stream, &grp, buffer, sizeof (buffer));
}

/* Return one entry from the password file (re-entrant version).  */
int
getgrent_r (struct group *result_buf, char *buffer, size_t buflen,
	    struct group **result)
{
  PTHREAD_SAFE_CANCELLATION

  return fgetgrent_r (stream, result_buf, buffer, buflen, result);
}
