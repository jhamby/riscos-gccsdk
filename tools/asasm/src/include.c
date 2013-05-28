/* 
 * AsAsm an assembler for ARM
 * Copyright (c) Andy Duplain, August 1992.
 * Converted to RISC OS by Niklas Röjemo
 * Copyright (c) 2002-2013 GCCSDK Developers
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */

#include "config.h"

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct IncludeDir
{
  struct IncludeDir *nextP;
  char dirP[];
} IncludeDir_t;

static IncludeDir_t *oIncludeDirListP;

void
Include_PrepareForPhase (Phase_e phase)
{
  switch (phase)
    {
      case eStartUp:
      case ePassOne:
      case ePassTwo:
      case eOutput:
	break;

      case eCleanUp:
	{
	  for (const IncludeDir_t *inclDirP = oIncludeDirListP; inclDirP != NULL; /* */)
	    {
	      const IncludeDir_t *nextInclDirP = inclDirP->nextP;
	      free ((void *)inclDirP);
	      inclDirP = nextInclDirP;
	    }
	  oIncludeDirListP = NULL;
	  break;
	}
    }
}


void
Include_Add (const char *inclDirP)
{
  IncludeDir_t **prevInclDirPP = &oIncludeDirListP;
  while (*prevInclDirPP != 0)
    {
      if (!strcmp ((*prevInclDirPP)->dirP, inclDirP))
	return; /* already in list */
      prevInclDirPP = &(*prevInclDirPP)->nextP;
    }      

  /* Need to add to the include directory list.  */
  size_t inclDirSize = strlen (inclDirP) + 1;
  IncludeDir_t *newInclDirP = malloc (sizeof (IncludeDir_t) + inclDirSize);
  if (newInclDirP == NULL)
    Error_OutOfMem ();
  newInclDirP->nextP = NULL;
  memcpy (newInclDirP->dirP, inclDirP, inclDirSize);
  ((IncludeDir_t *)prevInclDirPP)->nextP = newInclDirP;

  /* Strip trailing dir separator */
  if (inclDirSize >= 2 && newInclDirP->dirP[inclDirSize - 2] == NAT_DIR_CHR)
    newInclDirP->dirP[inclDirSize - 2] = '\0';
}


/**
 * Returns ASFile structure of given filename (possibly via suffix swapping).
 * User supplied include paths are always taken into account in order to
 * find the given filename.
 * \param fileName Filename of file which needs to be opened.
 * \param asFileP ASFile object to be filled in for given filename.
 * \return false for success, true otherwise (like unexisting file).
 */
bool
Include_Find (const char *fileName, ASFile *asFileP)
{
  char outBuf[PATH_MAX];
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
  if (strchr (fileName, ':'))
    return true;

  /* Try to find the file via the user supplied include paths.  */
  for (const IncludeDir_t *incDirP = oIncludeDirListP; incDirP != NULL; incDirP = incDirP->nextP)
    {
      char incpath[PATH_MAX];
      bool state[3] = { false, false, false };
      const char *out[3];

      do
	{
	  out[0] = FN_AnyToNative (fileName, 0, outBuf, sizeof (outBuf),
				   &state[0], eA_Dot_B);
	  if (out[0])
	    {
	      snprintf (incpath, sizeof (incpath), "%s" NAT_DIR_STR "%s", incDirP->dirP, out[0]);
	      incpath[sizeof (incpath) - 1] = '\0';
	      if (!ASFile_Create (incpath, asFileP))
		return false;
	    }

	  out[1] = FN_AnyToNative (fileName, 0, outBuf, sizeof (outBuf),
				   &state[1], eB_DirSep_A);
	  if (out[1])
	    {
	      snprintf (incpath, sizeof (incpath), "%s" NAT_DIR_STR "%s", incDirP->dirP, out[1]);
	      incpath[sizeof (incpath) - 1] = '\0';
	      if (!ASFile_Create (incpath, asFileP))
		return false;
	    }

	  out[2] = FN_AnyToNative (fileName, 0, outBuf, sizeof (outBuf),
				   &state[2], eA_Slash_B);
	  if (out[2])
	    {
	      snprintf (incpath, sizeof (incpath), "%s" NAT_DIR_STR "%s", incDirP->dirP, out[2]);
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
 * Opens file for read.
 * \param fileNameP Filename as specified by user or in assembler source.
 * This is before any translation to a real existing filename.
 * \param asFileP ASFile object to open a read-only std C FILE for.
 * \return When non-NULL, a std C file stream pointer.
 */
FILE *
Include_OpenForRead (const char *fileNameP, const ASFile *asFileP)
{
  if (option_verbose > 1)
    fprintf (stderr, "Open '%s' as '%s' for read: ", fileNameP, asFileP->canonName);
  FILE *fp = fopen (asFileP->canonName, "r");
  if (option_verbose > 1)
    fprintf (stderr, fp ? "ok\n" : "not ok\n");
  return fp;
}
