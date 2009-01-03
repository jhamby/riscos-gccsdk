/* 
 * AS an assembler for ARM
 * Copyright (c) Andy Duplain, August 1992.
 * Converted to RISC OS by Niklas Röjemo
 * Copyright (c) 2002-2008 GCCSDK Developers
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * include.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#if !defined(__riscos__) || defined(__TARGET_UNIXLIB__)
# include <sys/param.h>		/* for MAXPATHLEN */
# include <fcntl.h>
# include <unistd.h>
#else
# define MAXPATHLEN 256
#endif

#ifdef __TARGET_UNIXLIB__
#include <unixlib/local.h>
#endif

#include "error.h"
#include "include.h"
#include "os.h"
#include "rname.h"
#include "main.h"

#define DIR '/'

static const char **incDirPP;
static int incDirCurSize;
static int incDirMaxSize;

int
addInclude (const char *path)
{
  int i;
  size_t len;
  char *newPath;

  for (i = 0; i < incDirCurSize; i++)
    if (strcmp (incDirPP[i], path) == 0)
      return 0; /* already in list */

  /* Need to add to the list */
  if (incDirCurSize == incDirMaxSize)
    {
      int newDirMaxSize = 2*incDirMaxSize + 3;
      incDirPP = (const char **)realloc (incDirPP, newDirMaxSize * sizeof (const char *));
      if (incDirPP == NULL)
        {
          incDirMaxSize = incDirCurSize = 0;
	  errorOutOfMem ("addInclude");
	  return -1;
        }
      incDirMaxSize = newDirMaxSize;
    }

  newPath = strdup (path);
  if (newPath == NULL)
    {
      errorOutOfMem ("addInclude");
      return -1;
    }
  /* Strip trailing DIR separator */
  len = strlen (newPath);
  if (newPath[len] == DIR)
    newPath[len] = '\0';
  incDirPP[incDirCurSize++] = newPath;
#if (defined DEBUG) && (DEBUG > 0)
  fprintf (stderr, "addInclude: added %s\n", newPath);
#endif

  return 0;
}


static FILE *
openInclude (const char *filename, const char *mode, const char **strdupFilename)
{
  FILE *fp;

  if ((fp = fopen (filename, mode)) == NULL)
    {
#if defined(__riscos__) && defined(__TARGET_UNIXLIB__)
      /* Try again, without suffix swapping */
      __set_riscosify_control(__get_riscosify_control () | __RISCOSIFY_NO_SUFFIX);
      fp = fopen (filename, mode);
      __set_riscosify_control(__get_riscosify_control () & ~__RISCOSIFY_NO_SUFFIX);
#endif
    }

  if (fp == NULL)
      *strdupFilename = NULL;
  else
#ifndef __riscos__
    *strdupFilename = strdup (filename);
#else
    *strdupFilename = CanonicalisePath (filename);
#endif

  return fp;
}


FILE *
getInclude (const char *file, const char *mode, const char **strdupFilename)
{
  int i;
#ifdef __riscos__
  FILE *fp;
#else
  char *filename;
#endif
  char *apcs;

  /* TODO: Replace this with a generic getenv().
   * This will require that we actually parse the path components,
   * and replace any instances of <...> with the result of getenv(...),
   * if such a thing exists. If it doesn't, leave <...> alone.
   */
  if ((apcs = strstr(file, "<APCS>")))
    {
      /* APCS-32 is 1 byte longer than <APCS>, so add one to the length */
      char *apcs_path = alloca(strlen(file) + 1 + 1 /* \0 */);
      memcpy(apcs_path, file, apcs - file);
      strcpy(apcs_path + (apcs - file), apcs_32bit ? "APCS-32" : "APCS-26");
      strcpy(apcs_path + (apcs - file) + 7, apcs + 6);

      file = apcs_path;
    }

#ifndef __riscos__
  filename = rname (file);
#endif

  *strdupFilename = NULL;
#ifndef __riscos__
  if (access (filename, F_OK) == 0)
    return openInclude (filename, mode, strdupFilename);
  else
    {
      if (filename[0] == '.' && filename[1] == '/')
        filename += 2; /* Skip / */
      else if (strchr(file, ':'))
        {
          /* Try presuming everything is a directory.  This is for the benefit of paths like Hdr:APCS.Common */
          apcs = filename;

          while (*apcs)
            {
              if (*apcs == '.') *apcs = '/';
              apcs++;
            }

            if (access (filename, F_OK) == 0)
              return openInclude (filename, mode, strdupFilename);
        }
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

  for (i = 0; i < incDirCurSize; i++)
    {
      char incpath[MAXPATHLEN];
      sprintf (incpath, "%s%c%s", incDirPP[i], DIR, file);
#if (defined DEBUG) && (DEBUG > 0)
      fprintf (stderr, "getInclude: Searching for %s\n", incpath);
#endif

#ifndef __riscos__
      if (access (incpath, F_OK) == 0)
        return openInclude (incpath, mode, strdupFilename);
#else
      if ((fp = openInclude (incpath, mode, strdupFilename)) != NULL)
        return fp;
#endif
    }

  return NULL;
}
