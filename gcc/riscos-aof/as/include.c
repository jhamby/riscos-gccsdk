/*
 * include.c
 *
 * (c) Andy Duplain, August 1992.
 * Converted to RISC OS by Niklas Röjemo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(CROSS_COMPILE) || defined(UNIXLIB)
# include <sys/param.h>		/* for MAXPATHLEN */
# include <fcntl.h>
# include <unistd.h>
#else
# define MAXPATHLEN 256
#endif

#ifdef UNIXLIB
#include <unixlib/local.h>
#endif

#include "error.h"
#include "include.h"
#include "os.h"
#include "rname.h"
#include "main.h"

#define INCDIRMAX  (40)
#define DIR '/'

static char *incdir[INCDIRMAX];

int
initInclude (void)
{
  int i;

  for (i = 0; i < INCDIRMAX; i++)
    incdir[i] = NULL;
  return 0;
}

int
addInclude (const char *path)
{
  int i;

  for (i = 0; i < INCDIRMAX; i++)
    if (incdir[i])
      if (strcmp (incdir[i], path) == 0)
	return 0;		/* already in list */

  for (i = 0; i < INCDIRMAX; i++)
    if (!incdir[i])
      {
        int len;
	if ((incdir[i] = strdup (path)) == NULL)
	  {
	    errorOutOfMem("addInclude");
	    return -1;
	  }
	len = strlen (incdir[i]);

	if (*(incdir[i] + len) == DIR)
	  *(incdir[i] + len) = '\0';
#if (defined DEBUG) && (DEBUG > 0)
	fprintf (stderr, "addInclude: added %s\n", incdir[i]);
#endif


	return 0;
      }

  fprintf (stderr, "addInclude: maximum number of include paths exceeded (%d)\n",
	   INCDIRMAX);

  return -1;
}


static FILE *
openInclude (const char *filename, const char *mode, const char **strdupFilename)
{
  FILE *fp;

  if ((fp = fopen (filename, mode)) == NULL)
    {
#ifndef CROSS_COMPILE
      /* Try again, without suffix swapping */
      __set_riscosify_control(__get_riscosify_control () | __RISCOSIFY_NO_SUFFIX);
      fp = fopen (filename, mode);
      __set_riscosify_control(__get_riscosify_control () & ~__RISCOSIFY_NO_SUFFIX);
#endif
    }

  if (fp == NULL)
      *strdupFilename = NULL;
  else
#ifdef CROSS_COMPILE
    *strdupFilename = strdup (filename);
#else
    *strdupFilename = CanonicalisePath (filename);
#endif

  return fp;
}


FILE *
getInclude (const char *filename, const char *mode, const char **strdupFilename)
{
  int i;
#ifndef CROSS_COMPILE
  FILE *fp;
#endif
  char incpath[MAXPATHLEN];
#if defined (CROSS_COMPILE)
  const char *file = rname (filename);
#else
  const char *file = filename;
#endif

  *strdupFilename = NULL;
#ifdef CROSS_COMPILE
  if (access (file, F_OK) == 0)
    return openInclude (file, mode, strdupFilename);
  else
    {
      if (file[0] == '.' && file[1] == '/')
	file += 2;		/* Skip ./ */
    }
#else
  if ((fp = openInclude (file, mode, strdupFilename)) != NULL)
    return fp;
  else
    {
      if (file[0] == '@' && file[1] == '.')
	file += 2;		/* Skip @. */
    }
#endif

  for (i = 0; i < INCDIRMAX; i++)
    {
#if (defined DEBUG) && (DEBUG > 0)
      fprintf( stderr, "getInclude: Searching for %s\n", incpath );
#endif

      if (incdir[i] != NULL)
        {
	  sprintf (incpath, "%s%c%s", incdir[i], DIR, file);
#ifdef CROSS_COMPILE
	  if (access (incpath, F_OK) == 0)
	    return openInclude (incpath, mode, strdupFilename);
#else
	  if ((fp = openInclude (incpath, mode, strdupFilename)) != NULL)
	    return fp;
#endif
	}
    }

  return NULL;
}
