/* 
 * AS an assembler for ARM
 * Copyright (c) Andy Duplain, August 1992.
 * Converted to RISC OS by Niklas Röjemo
 * Copyright (c) 2002-2012 GCCSDK Developers
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

#include "config.h"

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
 * Returns ASFile structure of given filename (possibly via suffix swapping).
 * \param fileName Filename of file which needs to be opened.
 * \param asFileP ASFile object to be filled in for given filename.
 * \param inc When true, consider user support include paths.
 * \return false for success, true otherwise (like unexisting file).
 */
bool
Include_Find (const char *fileName, ASFile *asFileP, bool inc)
{
  char outBuf[MAXPATHLEN];
  for (unsigned pathidx = 0; /* */; ++pathidx)
    {
      bool state[3] = { false, false, false };
      const char *out[3];

      do
	{
	  out[0] = FN_AnyToNative (fileName, pathidx, outBuf, sizeof (outBuf),
				   &state[0], eA_Dot_B);
	  if (out[0] && !ASFile_Create (out[0], asFileP))
	    return false;

	  out[1] = FN_AnyToNative (fileName, pathidx, outBuf, sizeof (outBuf),
				   &state[1], eB_DirSep_A);
	  if (out[1] && !ASFile_Create (out[1], asFileP))
	    return false;

	  out[2] = FN_AnyToNative (fileName, pathidx, outBuf, sizeof (outBuf),
				   &state[2], eA_Slash_B);
	  if (out[2] && !ASFile_Create (out[2], asFileP))
	    return false;

	  assert (state[0] == state[1] && state[0] == state[2]);
	} while (out[0] && out[1] && out[2] && state[0]);

      if (out[0] == NULL && out[1] == NULL && out[2] == NULL)
	break;
    }

  /* We're done when we can't use user supplied include paths or when in
     given filename there is a path (like "Hdr:APCS.Common.").  */
  if (!inc || strchr (fileName, ':'))
    return true;

  /* Try to find the file via the user supplied include paths.  */
  for (unsigned i = 0; i != incDirCurSize; i++)
    {
      char incpath[MAXPATHLEN];
      bool state[3] = { false, false, false };
      const char *out[3];

      do
	{
	  out[0] = FN_AnyToNative (fileName, 0, outBuf, sizeof (outBuf),
				   &state[0], eA_Dot_B);
	  if (out[0])
	    {
	      snprintf (incpath, sizeof (incpath), "%s" NAT_DIR_STR "%s", incDirPP[i], out[0]);
	      incpath[sizeof (incpath) - 1] = '\0';
	      if (!ASFile_Create (incpath, asFileP))
		return false;
	    }

	  out[1] = FN_AnyToNative (fileName, 0, outBuf, sizeof (outBuf),
				   &state[1], eB_DirSep_A);
	  if (out[1])
	    {
	      snprintf (incpath, sizeof (incpath), "%s" NAT_DIR_STR "%s", incDirPP[i], out[1]);
	      incpath[sizeof (incpath) - 1] = '\0';
	      if (!ASFile_Create (incpath, asFileP))
		return false;
	    }

	  out[2] = FN_AnyToNative (fileName, 0, outBuf, sizeof (outBuf),
				   &state[2], eA_Slash_B);
	  if (out[2])
	    {
	      snprintf (incpath, sizeof (incpath), "%s" NAT_DIR_STR "%s", incDirPP[i], out[2]);
	      incpath[sizeof (incpath) - 1] = '\0';
	      if (!ASFile_Create (incpath, asFileP))
		return false;
	    }

	  assert (state[0] == state[1] && state[0] == state[2]);
        } while (out[0] && out[1] && out[2] && state[0]);

      if (out[0] == NULL && out[1] == NULL && out[2] == NULL)
	break;
    }

  return true;
}

/**
 * Opens file with given filename (possibly via suffix swapping tricks)
 * and optionally returns a possibly better qualified filename.
 * \param fileName Filename of file which needs to be opened.
 * \param asFileP ASFile object to be filled in for given filename.
 * \param inc When true, consider user support include paths.
 * \return When non-NULL, a std C file stream pointer.
 */
FILE *
Include_Get (const char *fileName, ASFile *asFileP, bool inc)
{
  if (Include_Find (fileName, asFileP, inc))
    return NULL;

  if (option_verbose > 1)
    fprintf (stderr, "Open '%s' as '%s' for read: ", fileName, asFileP->canonName);
  FILE *fp = fopen (asFileP->canonName, "r");
  if (option_verbose > 1)
    fprintf (stderr, fp ? "ok\n" : "not ok\n");
  return fp;
}
