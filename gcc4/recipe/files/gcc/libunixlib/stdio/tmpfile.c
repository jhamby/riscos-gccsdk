/* tmpfile (), tmpfile64 (), tmpnam (), tmpnam_r (), mktemp (), tempnam ()
 * Copyright (c) 2000-2013 UnixLib Developers
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <unixlib/local.h>

#include <internal/local.h>
#include <internal/unix.h>
#include <pthread.h>

/* These are the characters used in temporary filenames. Increasing
   this string will exceed the maximum unsigned integer size. */
static const char letters[] = "abcdefghijklmnopqrstuvwxyz0123456789";

static unsigned int offset = 0;

/* A seriously funky temporary filename generator.  */

static char *
generate_temporary_filename (char *buf, const char *dir,
			     const char *file_template)
{
  const unsigned int maxidx = (sizeof (letters) - 1) * (sizeof (letters) - 1)
    * (sizeof (letters) - 1) * (sizeof (letters) - 1)
    * (sizeof (letters) - 1) * (sizeof (letters) - 1);

  /* Create a pathname, include 'dir' if not null. */
  char *s = buf;
  if (dir)
    {
      while ((*s++ = *dir++))
	;
      s[-1] = (__get_riscosify_control () & __RISCOSIFY_NO_PROCESS) ? '.' : '/';
    }
  /* Now for the filename template. This must have six 'X' on the end.  */
  size_t template_len = strlen (file_template);
  if (template_len < 6)
    {
      __set_errno (EINVAL);
      return NULL;
    }
  template_len -= 6;
  while (template_len--)
    *s++ = *file_template++;
  if (file_template[0] != 'X' || file_template[1] != 'X'
      || file_template[2] != 'X' || file_template[3] != 'X'
      || file_template[4] != 'X' || file_template[5] != 'X')
    {
      __set_errno (EINVAL);
      return NULL;
    }

  const unsigned int idx_start = (__ul_global.time[0] + offset++) % maxidx;
  unsigned int idx = idx_start;
  while (1)
    {
      unsigned int i = idx;
      s[0] = letters[i % (sizeof (letters) - 1)];
      i /= sizeof (letters) - 1;
      s[1] = letters[i % (sizeof (letters) - 1)];
      i /= sizeof (letters) - 1;
      s[2] = letters[i % (sizeof (letters) - 1)];
      i /= sizeof (letters) - 1;
      s[3] = letters[i % (sizeof (letters) - 1)];
      i /= sizeof (letters) - 1;
      s[4] = letters[i % (sizeof (letters) - 1)];
      i /= sizeof (letters) - 1;
      s[5] = letters[i];
      s[6] = '\0';

      /* Check for filename existence.  */
      if (!__object_exists_ux (buf))
	return buf;

      if (++idx == maxidx)
	idx = 0;
      if (idx == idx_start)
	break; /* Couldn't create a suitable temporary filename.  */
    }

  __set_errno (EEXIST);
  return NULL;
}


static const char *
get_tmpdir (void)
{
  return (__get_riscosify_control () & __RISCOSIFY_NO_PROCESS) ? "<Wimp$ScrapDir>" : P_tmpdir;
}


static int
isdir (const char *dir)
{
  if (*dir == '\0')
    return 0;
  return ((__get_riscosify_control () & __RISCOSIFY_NO_PROCESS) ? __object_exists_ro (dir) : __object_exists_ux (dir)) == 2;
}


FILE *
tmpfile (void)
{
  PTHREAD_UNSAFE

  const char *name = tmpnam (NULL);

  FILE *result;
  if ((result = fopen (name, "wb+")) == NULL)
    unlink (name);
  else
    {
#if 1
      /* Inline the fcntl call.  */
      getfd (result->fd)->fflag |= O_UNLINKED;
#else
      /* This should never fail. */
      if (fcntl (result->fd, F_SETUNL, 1))
	{
	  fclose (result);
	  unlink (name);
	  result = NULL;
	}
#endif
    }

  return result;
}
#if __UNIXLIB_LFS64_SUPPORT
#  error "64-bit LFS support missing."
#else
/* FIXME: indicate 4 GByte-1 limit.  */
strong_alias (tmpfile, tmpfile64)
#endif


static char __tmpbuf[L_tmpnam];

/* Defined by POSIX as not threadsafe when passed a NULL argument */
char *
tmpnam (char *buf)
{
  if (!buf)
    buf = __tmpbuf;

  return generate_temporary_filename (buf, get_tmpdir (), "__XXXXXX");
}


/* Re-entrant version of tmpnam().  */
char *
tmpnam_r (char *buf)
{
  if (!buf)
    {
      __set_errno (EINVAL);
      return NULL;
    }

  return generate_temporary_filename (buf, get_tmpdir (), "__XXXXXX");
}


char *
mktemp (char *file_template)
{
  return generate_temporary_filename (file_template, NULL, file_template);
}


char *
tempnam (const char *dir, const char *prefix)
{
  /* There are 3 strategies for choosing the temporary directory.  */

  /* 1. Use the name in the environment variable 'TMPDIR', if it is
     defined.  */
  const char *d = getenv ("TMPDIR");
  if (d != NULL && !isdir (d))
    d = NULL;

  /* 2. Use the 'dir' argument, if it is not a null pointer.  */
  if (d == NULL && dir != NULL && isdir (dir))
    d = dir;

  /* 3. The value of the 'P_tmpdir' macro.  */
  if (d == NULL && isdir (get_tmpdir ()))
    d = get_tmpdir ();

  if (d == NULL)
    {
      __set_errno (ENOENT);
      return NULL;
    }

  char pref[12];
  sprintf (pref, "%.4sXXXXXX", prefix ? prefix : "temp");

  char buf[L_tmpnam];
  if (generate_temporary_filename (buf, d, pref))
    return strdup (buf);
  return NULL;
}
