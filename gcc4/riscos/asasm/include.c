/* 
 * AS an assembler for ARM
 * Copyright (c) Andy Duplain, August 1992.
 * Converted to RISC OS by Niklas Röjemo
 * Copyright (c) 2002-2011 GCCSDK Developers
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

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(__riscos__) || defined(__TARGET_UNIXLIB__)
#  include <sys/param.h>		/* for MAXPATHLEN */
#else
#  define MAXPATHLEN 1024
#endif

#include "error.h"
#include "filename.h"
#include "include.h"
#include "os.h"
#include "main.h"

#ifdef __riscos__
#  define NAT_DIR_STR "."
#  define NAT_DIR_CHR '.'
#else
#  define NAT_DIR_STR "/"
#  define NAT_DIR_CHR '/'
#endif

static const char **incDirPP;
static unsigned incDirCurSize;
static unsigned incDirMaxSize;

void
Include_Add (const char *path)
{
  for (unsigned i = 0; i != incDirCurSize; i++)
    if (strcmp (incDirPP[i], path) == 0)
      return; /* already in list */

  /* Need to add to the list */
  if (incDirCurSize == incDirMaxSize)
    {
      size_t newDirMaxSize = 2*incDirMaxSize + 3;
      incDirPP = (const char **)realloc (incDirPP, newDirMaxSize * sizeof (const char *));
      if (incDirPP == NULL)
        {
          incDirMaxSize = incDirCurSize = 0;
	  errorOutOfMem ();
        }
      incDirMaxSize = newDirMaxSize;
    }

  char *newPath = strdup (path);
  if (newPath == NULL)
    errorOutOfMem ();
  /* Strip trailing dir separator */
  size_t len = strlen (newPath);
  if (newPath[len] == NAT_DIR_CHR)
    newPath[len] = '\0';
  incDirPP[incDirCurSize++] = newPath;
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
Include_Open (const char *filename, const char **strdupFilename)
{
  if (option_verbose > 1)
    fprintf (stderr, "Trying to open '%s' for read: ", filename);
  FILE *fp = fopen (filename, "r");
  if (option_verbose > 1)
    fprintf (stderr, fp ? "success\n" : "failed\n");

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
 * \param inc When true, consider user support include paths.
 * \return When non-NULL, a std C file stream pointer.
 */
FILE *
Include_Get (const char *file, const char **strdupFilename, bool inc)
{
  FILE *fp;
  char outBuf[MAXPATHLEN];
  for (unsigned pathidx = 0; /* */; ++pathidx)
    {
      bool state[3] = { false, false, false };
      const char *out[3];

      do
	{
	  out[0] = FN_AnyToNative (file, pathidx, outBuf, sizeof (outBuf),
				   &state[0], eA_Dot_B);
	  if (out[0] && (fp = Include_Open (out[0], strdupFilename)) != NULL)
	    return fp;

	  out[1] = FN_AnyToNative (file, pathidx, outBuf, sizeof (outBuf),
				   &state[1], eB_Dot_A);
	  if (out[1] && (fp = Include_Open (out[1], strdupFilename)) != NULL)
	    return fp;

	  out[2] = FN_AnyToNative (file, pathidx, outBuf, sizeof (outBuf),
				   &state[2], eA_Slash_B);
	  if (out[2] && (fp = Include_Open (out[2], strdupFilename)) != NULL)
	    return fp;

	  assert (state[0] == state[1] && state[0] == state[2]);
	} while (out[0] && out[1] && out[2] && state[0]);

      if (out[0] == NULL && out[1] == NULL && out[2] == NULL)
	break;
    }

  /* We're done when we can't use user supplied include paths or when in
     given filename there is a path (like "Hdr:APCS.Common.").  */
  if (!inc || strchr (file, ':'))
    return NULL;

  /* Try to find the file via the user supplied include paths.  */
  for (unsigned i = 0; i != incDirCurSize; i++)
    {
      char incpath[MAXPATHLEN];
      bool state[3] = { false, false, false };
      const char *out[3];

      do
	{
	  out[0] = FN_AnyToNative (file, 0, outBuf, sizeof (outBuf),
				   &state[0], eA_Dot_B);
	  if (out[0])
	    {
	      snprintf (incpath, sizeof (incpath), "%s" NAT_DIR_STR "%s", incDirPP[i], out[0]);
	      incpath[sizeof (incpath) - 1] = '\0';
	      if ((fp = Include_Open (incpath, strdupFilename)) != NULL)
		return fp;
	    }

	  out[1] = FN_AnyToNative (file, 0, outBuf, sizeof (outBuf),
				   &state[0], eB_Dot_A);
	  if (out[1])
	    {
	      snprintf (incpath, sizeof (incpath), "%s" NAT_DIR_STR "%s", incDirPP[i], out[1]);
	      incpath[sizeof (incpath) - 1] = '\0';
	      if ((fp = Include_Open (incpath, strdupFilename)) != NULL)
		return fp;
	    }

	  out[2] = FN_AnyToNative (file, 0, outBuf, sizeof (outBuf),
				   &state[0], eA_Slash_B);
	  if (out[2])
	    {
	      snprintf (incpath, sizeof (incpath), "%s" NAT_DIR_STR "%s", incDirPP[i], out[2]);
	      incpath[sizeof (incpath) - 1] = '\0';
	      if ((fp = Include_Open (incpath, strdupFilename)) != NULL)
		return fp;
	    }

	  assert (state[0] == state[1] && state[0] == state[2]);
        } while (out[0] && out[1] && out[2] && state[0]);

      if (out[0] == NULL && out[1] == NULL && out[2] == NULL)
	break;
    }

  return NULL;
}
