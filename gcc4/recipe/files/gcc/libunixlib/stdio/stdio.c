/* UnixLib stdio stdin/stdout/stderr initialisation and finalisation.
   Copyright (c) 2000-2010 UnixLib Developers.  */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <internal/unix.h>

FILE *__iob_head = NULL;
FILE *__stdin, *__stdout, *__stderr;

void
__stdioinit (void)
{
  /* Open the standard streams.  */
  if ((stdin = fdopen (0, "r")) == NULL
      || setvbuf (stdin, NULL, _IOLBF, BUFSIZ) != 0
      || (stdout = fdopen (1, "w")) == NULL
      || setvbuf (stdout, NULL, _IOLBF, BUFSIZ) != 0
      || (stderr = fdopen (2, "w")) == NULL
      || setvbuf (stderr, NULL, _IONBF, BUFSIZ) != 0)
    __unixlib_fatal ("Failed to init stdio");
}

void
__stdioexit (void)
{
  /* Close all streams.  */
  fclose (NULL);
  __stderr = __stdout = __stdin = NULL;
}
