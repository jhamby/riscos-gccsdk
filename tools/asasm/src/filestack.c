/*
 * AS an assembler for ARM
 * Copyright (c) Andy Duplain, August 1992.
 *     Added line numbers  Niklas Röjemo
 *     Added filenames     Darren Salt
 * Copyright (c) 2000-2013 GCCSDK Developers
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

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#include <string.h>

#include "error.h"
#include "filestack.h"
#include "include.h"
#include "input.h"
#include "local.h"
#include "main.h"
#include "os.h"
#include "whileif.h"

FileNameList *gFileNameListP; // FIXME: needs freeing !

PObject gPOStack[PARSEOBJECT_STACK_SIZE];
PObject *gCurPObjP = NULL; /**< Current parsable object.  */

static bool File_GetLine (char *bufP, size_t bufSize);

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
	Error_OutOfMem ();
      resultP->nextP = gFileNameListP;
      strcpy (resultP->fileName, fileNameP);
      gFileNameListP = resultP;
    }
  free ((void *)fileNameP);

  return resultP->fileName;
}


/**
 * \return The number of outstanding macro invocations.
 */
unsigned
FS_GetMacroDepth (void)
{
  assert (gCurPObjP != NULL);
  unsigned macroDepth = 0;
  for (const PObject *pObjP = gCurPObjP; pObjP != gPOStack; --pObjP)
    {
      if (pObjP->type == POType_eMacro)
	++macroDepth;
    }
  return macroDepth;
}


/**
 * Opens given file for immediate parsing.  Cancel using FS_PopPObject().
 * \param fileName Filename of assembler file to parse.
 * \return false for success, true for failure.
 * Similar to FS_PushMacroPObject().
 */
bool
FS_PushFilePObject (const char *fileName)
{
#ifdef DEBUG_FILESTACK
  ReportFSStack (__func__);
#endif

  PObject *newPObjP;
  unsigned int prevWhileIfDepth;
  if (gCurPObjP == NULL)
    {
      newPObjP = &gPOStack[0];
      prevWhileIfDepth = 0;
    }
  else if (gCurPObjP != &gPOStack[PARSEOBJECT_STACK_SIZE - 1])
    {
      newPObjP = &gCurPObjP[1];
      prevWhileIfDepth = gCurPObjP[0].whileIfCurDepth;
    }
  else
    Error_Abort ("Maximum file/macro nesting level reached (%d)", PARSEOBJECT_STACK_SIZE);

  ASFile asFile;
  newPObjP->d.file.fhandle = Include_Get (fileName, &asFile, true);
  if (newPObjP->d.file.fhandle == NULL)
    {
      Error (ErrorError, "Cannot open file \"%s\"", fileName);
      return true;
    }

  newPObjP->type = POType_eFile;
  newPObjP->fileName = StoreFileName (asFile.canonName);
  asFile.canonName = NULL; /* Ownership moved to StoreFileName().  */
  newPObjP->lineNum = 0;
  newPObjP->whileIfCurDepth = newPObjP->whileIfStartDepth = prevWhileIfDepth;
  newPObjP->GetLine = File_GetLine;
  newPObjP->lastLineSize = 0;

  ASFile_Free (&asFile);
  
  /* Set current file stack pointer.  All is ok now.  */
  gCurPObjP = newPObjP;
  return false;
}


/**
 * Similar to FS_PopMacroPObject().
 */
static void
FS_PopFilePObject (bool noCheck)
{
#ifdef DEBUG_FILESTACK
  ReportFSStack (__func__);
#endif

  FS_PopIfWhile (noCheck);

  if (!noCheck && option_verbose)
    fprintf (stderr, "Returning from file \"%s\"\n", gCurPObjP->fileName);

  gCurPObjP->fileName = NULL;
  fclose (gCurPObjP->d.file.fhandle);
  gCurPObjP->d.file.fhandle = NULL;
}


static bool
File_GetLine (char *bufP, size_t bufSize)
{
  gCurPObjP->lastLineSize = 0;
  while (1)
    {
      if (fgets (bufP, bufSize, gCurPObjP->d.file.fhandle) == NULL
	  || bufP[0] == '\0')
	return true;

      size_t lineLen = strlen (bufP);
      gCurPObjP->lastLineSize += lineLen;
      if (lineLen > 0 && bufP[lineLen - 1] == '\n')
	{
	  if (lineLen > 1 && bufP[lineLen - 2] == '\\')
	    {
	      bufP += lineLen - 2;
	      bufSize -= lineLen - 2;
	      gCurPObjP->lineNum++;
	    }
	  else
	    {
	      bufP[lineLen - 1] = '\0';
	      return false;
	    }
	}
      else if (feof (gCurPObjP->d.file.fhandle))
	return false;
      else
	Error_Abort ("Line too long");
    }
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
	Local_FinishMacro (noCheck);
	FS_PopMacroPObject (noCheck);
	break;

      default:
	assert (0);
	break;
    }

  if (gCurPObjP == &gPOStack[0])
    gCurPObjP = NULL;
  else
    --gCurPObjP;
}


/**
 * \return A non-NULL pointer of filename of the current parsing object.
 */
const char *
FS_GetCurFileName (void)
{
  if (gCurPObjP == NULL)
    return SourceFileName;
  return gCurPObjP->fileName;
}


/**
 * \return Current linenumber.
 */
unsigned
FS_GetCurLineNumber (void)
{
  return gCurPObjP ? gCurPObjP->lineNum : 0;
}


/**
 * {LINENUM} implementation.
 * \return Line number in the current source file.
 * FIXME: not 100% sure this is the correct implementation.
 */
unsigned
FS_GetBuiltinVarLineNum (void)
{
  assert (gCurPObjP != NULL);
  assert (gPOStack[0].type == POType_eFile);
  return gPOStack[0].lineNum;
}


/**
 * {LINENUMUP} implementation.
 * \return When used in a macro, returns the line number of the current macro.
 * This is the same as {LINENUM} when used in a non-macro context.
 * FIXME: not 100% sure this is the correct implementation.
 */
unsigned
FS_GetBuiltinVarLineNumUp (void)
{
  assert (gCurPObjP != NULL);
  if (gCurPObjP->type == POType_eMacro)
    return gCurPObjP->lineNum - gCurPObjP->d.macro.macro->startLineNum;
  assert (gPOStack[0].type == POType_eFile);
  return gPOStack[0].lineNum;
}


/**
 * {LINENUMUPPER} implementation.
 * \return When used in a macro, returns the line number of the top macro.
 * The value is same as {LINENUM} when used in a non-macro context.
 * FIXME: not 100% sure this is the correct implementation.
 */
unsigned
FS_GetBuiltinVarLineNumUpper (void)
{
  assert (gCurPObjP != NULL);
  for (const PObject *pObjP = &gPOStack[0]; pObjP != gCurPObjP; ++pObjP)
    {
      if (pObjP->type == POType_eMacro)
	return pObjP->lineNum - pObjP->d.macro.macro->startLineNum;
    }
  if (gCurPObjP->type == POType_eMacro)
    return gCurPObjP->lineNum - gCurPObjP->d.macro.macro->startLineNum;
  assert (gPOStack[0].type == POType_eFile);
  return gPOStack[0].lineNum;
}


#ifdef DEBUG_FILESTACK
void
ReportFSStack (const char *id)
{
  printf ("Current stack of parsable objects @ %s\n", id);
  if (gCurPObjP == NULL)
    {
      printf ("  - No parsable objects.\n");
      return;
    }

  const PObject *pObjP = gCurPObjP;
  do
    {
      switch (pObjP->type)
	{
	  case POType_eFile:
	    printf ("  - File: line num %d (if/while %d - %d): %s\n",
	            pObjP->lineNum, pObjP->whileIfStartDepth, pObjP->whileIfCurDepth,
	            pObjP->name);
	    break;

	  case POType_eMacro:
	    printf ("  - Macro: line num %d (if/while %d - %d): %s\n",
	            pObjP->lineNum, pObjP->whileIfStartDepth, pObjP->whileIfCurDepth,
	            pObjP->name);
	    break;

	  default:
	    printf ("  - Unknown parsable object.\n");
	    break;
	}
    } while (pObjP-- != &gPOStack[0]);
}
#endif
