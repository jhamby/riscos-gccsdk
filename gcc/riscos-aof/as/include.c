/*
 * include.c
 *
 * (c) Andy Duplain, August 1992.
 * Converted to RISC/OS by Niklas Röjemo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(CROSS_COMPILE) || defined(UNIXLIB)
#include <fcntl.h>
#include <unistd.h>
#endif

#include "include.h"
#include "uname.h"
#include "rname.h"

extern int dde;

#define INCDIRMAX  10

static char *incdir[INCDIRMAX];

int 
initInclude (void)
{
  int i;

  for (i = 0; i < INCDIRMAX; i++)
    incdir[i] = NULL;
  return (0);
}

int 
addInclude (const char *incpath)
{
  int i;
#if defined (UNIXLIB) || defined (CROSS_COMPILE)
  const char *path = incpath;
#else
  char *path = uname (incpath, dde);
#endif

  for (i = 0; i < INCDIRMAX; i++)
    if (incdir[i])
      if (strcmp (incdir[i], path) == 0)
	return (0);		/* already in list */

  for (i = 0; i < INCDIRMAX; i++)
    if (!incdir[i])
      {
	if ((incdir[i] = strdup (path)) != 0)
	  {
	    int len = strlen (incdir[i]);

	    if (*(incdir[i] + len) == DIR)
	      *(incdir[i] + len) = '\0';

	    return (0);
	  }
	else
	  {
	    fprintf (stderr, "addInclude: strdup failed\n");
	    return (-1);
	  }
      }

  fprintf (stderr, "addInclude: maximum number of include paths exceeded (%d)\n",
	   INCDIRMAX);

  return (-1);
}

FILE *
getInclude (const char *filename, const char *mode)
{
  int i;
#ifndef CROSS_COMPILE
  FILE *fp;
#endif
  char incpath[MAXPATHLEN];
#if defined (UNIXLIB)
  char *file = (char *) filename;
#elif defined (CROSS_COMPILE)
  char *file = rname (filename);
#else
  char *file = uname (filename, dde);
#endif

#ifdef CROSS_COMPILE
  if (access (file, F_OK) == 0)
    return fopen (file, mode);
  else
    {
      if (file[0] == '.' && file[1] == '/')
	file += 2;		/* Skip ./ */
    }
#else
  if ((fp = fopen (file, mode)) != 0)
    return fp;
  else
    {
      if (file[0] == '@' && file[1] == '.')
	file += 2;		/* Skip @. */
    }
#endif

  for (i = 0; i < INCDIRMAX; i++)
    if (incdir[i])
      {
	sprintf (incpath, "%s%c%s", incdir[i], DIR, file);
#ifdef CROSS_COMPILE
	if (access (incpath, F_OK) == 0)
	  return fopen (incpath, mode);
#else
	if ((fp = fopen (incpath, mode)) != 0)
	  return fp;
#endif
      }
  return NULL;
}
