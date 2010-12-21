/*
 * AS an assembler for ARM
 * Copyright (c) Andy Duplain, August 1992.
 *     Added line numbers  Niklas Röjemo
 *     Added filenames     Darren Salt
 * Copyright (c) 2000-2010 GCCSDK Developers
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
 * filestack.c
 */

#include "config.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#include <string.h>

#if defined(__TARGET_UNIXLIB__)
#  include <unixlib/local.h>
#endif

#include "error.h"
#include "filestack.h"
#include "include.h"
#include "input.h"
#include "main.h"
#include "os.h"
#include "whileif.h"

FileNameList *gFileNameListP; // FIXME: needs freeing !

PObject gPOStack[PARSEOBJECT_STACK_SIZE];
PObject *gCurPObjP; /**< Current parsable object.  */

static bool File_GetLine(char *bufP, size_t bufSize);

/**
 * In order to have permanent storage of filenames.
 * \param fileNameP Pointer to malloced filename.
 * \return pointer to same filename, no ownership.
 */
static const char *
StoreFileName (const char *fileNameP)
{
  if (fileNameP == NULL)
    return NULL;

  /* Check if we have already that filename stored.  */
  FileNameList *resultP;
  for (resultP = gFileNameListP;
       resultP != NULL && strcmp (resultP->fileName, fileNameP);
       resultP = resultP->nextP)
    /* */;
  if (resultP == NULL)
    {
      if ((resultP = malloc (offsetof (FileNameList, fileName) + strlen (fileNameP) + 1)) == NULL)
	errorOutOfMem ();
      resultP->nextP = gFileNameListP;
      strcpy (resultP->fileName, fileNameP);
      gFileNameListP = resultP;
    }
  free ((void *)fileNameP);

  return resultP->fileName;
}


/**
 * Opens given file for immediate parsing.  Cancel using FS_PopPObject().
 * \param fileName Filename of assembler file to parse.  May only be NULL when
 * at level 0 and this means reading from stdin.
 * Similar to FS_PushMacroPObject().
 */
void
FS_PushFilePObject (const char *fileName)
{
  if (gCurPObjP == &gPOStack[PARSEOBJECT_STACK_SIZE - 1])
    errorAbort ("Maximum file/macro nesting level reached (%d)", PARSEOBJECT_STACK_SIZE);

  if (gCurPObjP == NULL)
    {
      /* The first assembler file is special.  We're not going to use any
         include paths and we can also read from stdin.  */
      if (fileName != NULL)
	{
	  if ((gPOStack[0].d.file.fhandle = fopen (fileName, "r")) == NULL)
	    {
#if defined(__TARGET_UNIXLIB__)
	      /* Try again but this time with(out) suffix swapping.  */
	      __riscosify_control ^= __RISCOSIFY_NO_SUFFIX;
	      gPOStack[0].d.file.fhandle = fopen (fileName, "r");
	      __riscosify_control ^= __RISCOSIFY_NO_SUFFIX;
#endif
	    }

	  if (gPOStack[0].d.file.fhandle == NULL)
	    gPOStack[0].name = NULL;
	  else if ((gPOStack[0].name = StoreFileName (CanonicalisePath (fileName))) == NULL)
	    {
	      fclose (gPOStack[0].d.file.fhandle);
	      gPOStack[0].d.file.fhandle = NULL;
	    }
	}
      else
	{
	  /* Read from stdin.  */
	  gPOStack[0].d.file.fhandle = stdin;
	  gPOStack[0].name = NULL;
	}
      if (gPOStack[0].d.file.fhandle != NULL)
	gCurPObjP = &gPOStack[-1];
    }
  else
    {
      const char *fileNameP;
      gCurPObjP[1].d.file.fhandle = getInclude (fileName, &fileNameP);
      gCurPObjP[1].name = StoreFileName (fileNameP);
    }
  if (gCurPObjP == NULL || gCurPObjP[1].d.file.fhandle == NULL)
    {
      error (ErrorError, "Cannot open file \"%s\"", fileName);
      return;
    }
  gCurPObjP[1].lineNum = 0;
  gCurPObjP[1].if_depth = 0;
  gCurPObjP[1].whilestack = NULL;
  gCurPObjP[1].GetLine = File_GetLine;

  /* Increase current file stack pointer.  All is ok now.  */
  ++gCurPObjP;

  skiprest ();
}


/**
 * Similar to FS_PopMacroPObject().
 */
static void
FS_PopFilePObject (bool noCheck)
{
  FS_PopIfWhile (noCheck);

  if (!noCheck && option_verbose)
    fprintf (stderr, "Returning from include file \"%s\"\n", gCurPObjP->name);

  gCurPObjP->name = NULL;
  fclose (gCurPObjP->d.file.fhandle);
  gCurPObjP->d.file.fhandle = NULL;
}


/**
 * \param noCheck When true, no checking will be performed.
 */
void
FS_PopPObject (bool noCheck)
{
  if (gCurPObjP == NULL)
    return;

  switch (gCurPObjP->type)
    {
      case POType_eFile:
	FS_PopFilePObject (noCheck);
	break;
      case POType_eMacro:
	FS_PopMacroPObject (noCheck);
	break;
      default:
	errorAbort ("Internal FS_PopPObject: unknown case");
    }

  if (gCurPObjP == &gPOStack[0])
    gCurPObjP = NULL;
  else
    --gCurPObjP;
}


/**
 * Always return a non-NULL pointer of filename of the current parsing object.
 */
const char *
FS_GetCurFileName (void)
{
  if (gCurPObjP == NULL)
    return SourceFileName ? SourceFileName : "{standard input}";
  return gCurPObjP->name ? gCurPObjP->name : "{standard input}";
}


/**
 * Get current linenumber.
 */
int
FS_GetCurLineNumber (void)
{
  return gCurPObjP ? gCurPObjP->lineNum : 0;
}


static bool
File_GetLine (char *bufP, size_t bufSize)
{
  while (1)
    {
      if (fgets (bufP, bufSize, gCurPObjP->d.file.fhandle) == NULL
	  || bufP[0] == '\0')
	return true;

      size_t lineLen = strlen (bufP);
      if (lineLen > 0 && bufP[lineLen - 1] == '\n')
	{
	  if (lineLen > 1 && bufP[lineLen - 2] == '\\')
	    {
	      bufP += lineLen - 2;
	      bufSize -= lineLen - 2;;
	    }
	  else
	    {
	      bufP[lineLen - 1] = '\0';
	      return false;
	    }
	}
      else
	errorAbort ("Line too long");
    }
}
