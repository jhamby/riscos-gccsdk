/* tmpfile (), tmpnam (), tmpnam_r (), mktemp (), mkstemp (), tempnam ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <internal/local.h>
#include <internal/unix.h>
#include <pthread.h>

/* These are the characters used in temporary filenames. Increasing
   this string will exceed the maximum unsigned integer size. */
static const char letters[] = "abcdefghijklmnopqrstuvwxyz0123456789";

/* A seriously funky temporary filename generator.  */

static char *
generate_temporary_filename (char *buf, const char *dir,
			     const char *file_template)
{
  char *s = buf;
  unsigned long idx;
  const unsigned int maxidx = (sizeof (letters) - 1) * (sizeof (letters) - 1)
    * (sizeof (letters) - 1) * (sizeof (letters) - 1)
    * (sizeof (letters) - 1) * (sizeof (letters) - 1);
  int loop = 0;

  /* Create a pathname, include 'dir' if not null. */
  if (dir)
    {
      while ((*s++ = *dir++))
	;
      s--;
      *s++ = '/';
    }
  /* Now for the filename template. This must have six 'X' on the end.  */
  while ((*s++ = *file_template++))
    ;
  s -= 7;
  if (*s != 'X')
    {
      __set_errno (EINVAL);
      return NULL;
    }

  idx = __ul_global.time[0] % maxidx;
  while (1)
    {
      if (idx >= maxidx)
	{
	  idx = 1;
	  loop ++;
	}
      if (loop >= 2)
	/* Couldn't create a suitable temporary filename.  */
	break;

      s[0] = letters[idx % (sizeof (letters) - 1)];
      s[1] = letters[(idx / (sizeof (letters) - 1)) % (sizeof (letters) - 1)];
      s[2] = letters[(idx / ((sizeof (letters) - 1) * (sizeof (letters) - 1)))
		     % (sizeof (letters) - 1)];
      s[3] = letters[(idx / ((sizeof (letters) - 1) * (sizeof (letters) - 1)
			     * (sizeof (letters) - 1)))
		     % (sizeof (letters) - 1)];
      s[4] = letters[(idx / ((sizeof (letters) - 1) * (sizeof (letters) - 1)
			 * (sizeof (letters) - 1) * (sizeof (letters) - 1)))
		     % (sizeof (letters) - 1)];
      s[5] = letters[(idx / ((sizeof (letters) - 1) * (sizeof (letters) - 1)
			   * (sizeof (letters) - 1) * (sizeof (letters) - 1)
			     * (sizeof (letters) - 1)))
		     % (sizeof (letters) - 1)];

      s[6] = '\0';
      /* Check for filename existence.  */
      if (!__object_exists (buf))
	return buf;

      idx++;
    }

  __set_errno (EEXIST);
  return NULL;
}


FILE *
tmpfile (void)
{
  FILE *result;
  const char *name;

  PTHREAD_UNSAFE

  name = tmpnam (NULL);

  if ((result = fopen (name, "wb+")) == NULL)
    {
      unlink (name);
    }
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


static char __tmpbuf[L_tmpnam + 1];

/* Defined by POSIX as not threadsafe when passed a NULL argument */
char *
tmpnam (char *buf)
{
  if (!buf)
    buf = __tmpbuf;

  return generate_temporary_filename (buf, P_tmpdir, "__XXXXXX");
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

  return generate_temporary_filename (buf, P_tmpdir, "__XXXXXX");
}

char *
mktemp (char *file_template)
{
  return generate_temporary_filename (file_template, 0, file_template);
}

int
mkstemp (char *file_template)
{
  return open (mktemp (file_template), O_RDWR | O_CREAT | O_TRUNC, 0666);
}

char *
tempnam (const char *dir, const char *prefix)
{
  const char *d;
  char buf[L_tmpnam], *b;
  char pref[12];

  /* There are 4 strategies for choosing the temporary directory.  */

  /* 1. Use the name in the environment variable 'TMPDIR', if it is
     defined.  */
  d = getenv ("TMPDIR");
  if (d != NULL && !__isdir (d))
    d = NULL;

  /* 2. Use the 'dir' argument, if it is not a null pointer.  */
  if (d == NULL && dir != NULL && __isdir (dir))
    d = dir;

  /* 3. The value of the 'P_tmpdir' macro.  */
  if (d == NULL && __isdir (P_tmpdir))
    d = P_tmpdir;

  /* 4. Try '/tmp'.  */
  if (d == NULL && __isdir ("/tmp"))
    d = "/tmp";

  if (d == NULL)
    {
      __set_errno (ENOENT);
      return NULL;
    }

  sprintf (pref, "%.4sXXXXXX", (prefix) ? prefix : "temp");

  if (generate_temporary_filename (buf, d, pref))
    {
      /* Copy the temporary file into a malloc buffer and return it.  */
      b = (char *) malloc (strlen (buf) + 1);
      strcpy (b, buf);
      return b;
    }
  return NULL;
}
