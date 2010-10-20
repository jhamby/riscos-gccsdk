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
#  include <sys/param.h>		/* for MAXPATHLEN */
#else
#  define MAXPATHLEN 256
#endif

#ifdef __TARGET_UNIXLIB__
#  include <unixlib/local.h>
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
	  errorOutOfMem ();
        }
      incDirMaxSize = newDirMaxSize;
    }

  newPath = strdup (path);
  if (newPath == NULL)
    {
      errorOutOfMem ();
      return -1;
    }
  /* Strip trailing DIR separator */
  len = strlen (newPath);
  if (newPath[len] == DIR)
    newPath[len] = '\0';
  incDirPP[incDirCurSize++] = newPath;
#ifdef DEBUG
  fprintf (stderr, "addInclude: added %s\n", newPath);
#endif

  return 0;
}


/**
 * Opens file with given filename, or suffix swapped (when using UnixLib)
 * and optionally returns a possibly better qualified filename.
 * \param filename Filename of file which needs to be opened.
 * \param strdupFilename When non-NULL, is a pointer of a value containing
 * on return a malloc block holding the possibly better qualified filename.
 * \return When non-NULL, a std C file stream pointer.
 */
static FILE *
openInclude (const char *filename, const char **strdupFilename)
{
  FILE *fp;
  if ((fp = fopen (filename, "r")) == NULL)
    {
#if defined(__TARGET_UNIXLIB__)
      /* Try again but this time with(out) suffix swapping.  */
      __riscosify_control ^= __RISCOSIFY_NO_SUFFIX;
      fp = fopen (filename, "r");
      __riscosify_control ^= __RISCOSIFY_NO_SUFFIX;
#endif
    }

  if (strdupFilename != NULL)
    {
      if (fp == NULL)
	*strdupFilename = NULL;
      else
	{
	  if ((*strdupFilename = CanonicalisePath (filename)) == NULL)
	    {
	      fclose (fp);
	      fp = NULL;
	    }
	}
    }
  
  return fp;
}


/**
 * Opens file with given filename, or suffix swapped (when using UnixLib)
 * and optionally returns a possibly better qualified filename.
 * \param filename Filename of file which needs to be opened.
 * \param strdupFilename When non-NULL, is a pointer of a value containing
 * on return a malloc block holding the possibly better qualified filename.
 * \return When non-NULL, a std C file stream pointer.
 */
FILE *
getInclude (const char *file, const char **strdupFilename)
{
  if (strdupFilename)
    *strdupFilename = NULL;

#ifdef __riscos__
  const char *filename = file;
#else
  char *filename = rname (file);
#endif
  FILE *fp;
  if ((fp = openInclude (filename, strdupFilename)) != NULL)
    return fp;

  /* Try to find the file via the user supplied include paths.  */
#ifndef __riscos__
  if (filename[0] == '.' && filename[1] == '/')
    filename += 2; /* Skip ./ */
  else if (strchr(file, ':'))
    {
      /* Try presuming everything is a directory.  
         This is for the benefit of paths like Hdr:APCS.Common.  */
      char *dot;
      for (dot = filename; *dot; ++dot)
	{
	  if (*dot == '.')
	    *dot = '/';
	}
      
      return openInclude (filename, strdupFilename);
    }
#else
  if (filename[0] == '@' && filename[1] == '.')
    filename += 2; /* Skip @. */
#endif

  int i;
  for (i = 0; i < incDirCurSize; i++)
    {
      char incpath[MAXPATHLEN];
      snprintf (incpath, sizeof (incpath), "%s%c%s", incDirPP[i], DIR, filename);
      incpath[sizeof (incpath) - 1] = '\0';

#ifdef DEBUG
      fprintf (stderr, "getInclude: Searching for %s\n", incpath);
#endif

      if ((fp = openInclude (incpath, strdupFilename)) != NULL)
        return fp;
    }

  return NULL;
}
