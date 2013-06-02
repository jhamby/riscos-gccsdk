/*
 * AsAsm an assembler for ARM
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
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

typedef struct CachedFile
{
  const struct CachedFile *nextP;
  const char *fileNameP; /** Canonicalized filename of this cached file.  */
  size_t size; /** Size of this cached file.  */
  char bufP[]; /** Pointer to cached file in memory.  */
} CachedFile_t;

static size_t oFileCacheSize; /* In bytes.  */
static const CachedFile_t *oCachedFileListP;

FileNameList *gFileNameListP;

PObject gPOStack[PARSEOBJECT_STACK_SIZE];
PObject *gCurPObjP = NULL; /**< Current parsable object.  Points into
  gPOStack array.  */

static bool File_GetLine (char *bufP, size_t bufSize);
static bool CachedFile_GetLine (char *bufP, size_t bufSize);

void
FS_PrepareForPhase (Phase_e phase)
{
  switch (phase)
    {
      case eStartUp:
      case ePassOne:
      case ePassTwo:
	break;

      case eOutput:
	{
	  /* We can remove all cached files now.  */
	  for (const CachedFile_t *cachedFileP = oCachedFileListP; cachedFileP != NULL; /* */)
	    {
	      const CachedFile_t *nextCachedFileP = cachedFileP->nextP;
	      free ((void *)cachedFileP);
	      cachedFileP = nextCachedFileP;
	    }
	  oCachedFileListP = NULL;
	  break;
	}

      case eCleanUp:
	{
	  for (const FileNameList *fileNameP = gFileNameListP; fileNameP != NULL; /* */)
	    {
	      const FileNameList *nextFileNameP = fileNameP->nextP;
	      free ((void *)fileNameP);
	      fileNameP = nextFileNameP;
	    }
	  gFileNameListP = NULL;
	  break;
	}
    }
}


/**
 * Put given filename in a permanent storage of filenames.
 * \param fileNameP Pointer to canonicalized filename.  No ownership is
 * taken.
 * \return pointer to same filename, no ownership transfered.
 */
const char *
FS_StoreFileName (const char *fileNameP)
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
      if ((resultP = malloc (sizeof (FileNameList) + strlen (fileNameP) + 1)) == NULL)
	Error_OutOfMem ();
      resultP->nextP = gFileNameListP;
      strcpy (resultP->fileName, fileNameP);
      gFileNameListP = resultP;
    }

  return resultP->fileName;
}


static const CachedFile_t *
CachedFile_Lookup (const char *fileNameP, const ASFile *asFileP)
{
  for (const CachedFile_t *cachedFileP = oCachedFileListP;
       cachedFileP != NULL;
       cachedFileP = cachedFileP->nextP)
    {
      if (!strcmp (asFileP->canonName, cachedFileP->fileNameP))
	return cachedFileP;
    }

  /* Not in the cache so far.  If there is still enough place, pull it in
     the cache.  */
  if ((size_t)asFileP->size <= oFileCacheSize)
    {
      CachedFile_t *cachedFileP = malloc (sizeof (CachedFile_t) + asFileP->size);
      if (cachedFileP == NULL)
	return NULL; /* Don't give an error, just read the file from disc.  */
      FILE *fhandle = Include_OpenForRead (fileNameP, asFileP);
      if (fhandle == NULL)
	{
	  free (cachedFileP);
	  return NULL;
	}
      if (fread (cachedFileP->bufP, asFileP->size, 1, fhandle) != 1)
	{
	  fclose (fhandle);
	  free (cachedFileP);
	  return NULL;
	}
      fclose (fhandle);

      cachedFileP->nextP = oCachedFileListP;
      cachedFileP->fileNameP = FS_StoreFileName (asFileP->canonName);
      cachedFileP->size = asFileP->size;
      
      oFileCacheSize -= asFileP->size;
      oCachedFileListP = cachedFileP;
      return oCachedFileListP;
    }

  return NULL;
}


/**
 * Sets the maximum file cache size in MBytes.
 */
void
FS_SetFileCacheSize (unsigned sizeMByte)
{
  oFileCacheSize = sizeMByte <= SIZE_MAX / (1024 * 1024) ? (size_t)sizeMByte * 1024 * 1024 : SIZE_MAX;
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
 * \param fileNameP Filename of assembler file to parse.
 * \return false for success, true for failure.
 * Similar to FS_PushMacroPObject().
 */
bool
FS_PushFilePObject (const char *fileNameP)
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
  if (Include_Find (fileNameP, &asFile))
    {
      Error (ErrorError, "Cannot find file \"%s\"", fileNameP);
      ASFile_Free (&asFile);
      return true;
    }

  /* Try to retrieve file from cache.  */
  const CachedFile_t *cachedFileP = CachedFile_Lookup (fileNameP, &asFile);
  if (cachedFileP != NULL)
    {
      newPObjP->type = POType_eCachedFile;
      newPObjP->d.memory.curP = newPObjP->d.memory.startP = cachedFileP->bufP; 
      newPObjP->d.memory.endP = cachedFileP->bufP + cachedFileP->size; 
      newPObjP->GetLine = CachedFile_GetLine;
    }
  else
    {
      newPObjP->type = POType_eFile;
      if ((newPObjP->d.file.fhandle = Include_OpenForRead (fileNameP, &asFile)) == NULL)
	{
	  Error (ErrorError, "Cannot open file \"%s\"", asFile.canonName);
	  ASFile_Free (&asFile);
	  return true;
	}
      newPObjP->GetLine = File_GetLine;
    }
  newPObjP->fileName = FS_StoreFileName (asFile.canonName);
  newPObjP->lineNum = 0;
  newPObjP->whileIfCurDepth = newPObjP->whileIfStartDepth = prevWhileIfDepth;
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
  assert (gCurPObjP->type == POType_eFile || gCurPObjP->type == POType_eCachedFile); 

  FS_PopIfWhile (noCheck);

  if (!noCheck && option_verbose)
    fprintf (stderr, "Returning from file \"%s\"\n", gCurPObjP->fileName);

  gCurPObjP->fileName = NULL;
  if (gCurPObjP->type == POType_eFile)
    {
      fclose (gCurPObjP->d.file.fhandle);
      gCurPObjP->d.file.fhandle = NULL;
    }
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


static bool
CachedFile_GetLine (char *bufP, size_t bufSize)
{
  PObject_CachedFile * const memP = &gCurPObjP->d.memory;
  const char *curP = memP->curP;

  /* Are we at EOF ? */
  if (curP == memP->endP)
    return true;

  const char * const bufStartP = bufP;
  const char * const bufEndP = bufP + bufSize;
  while (1)
    {
      /* Read one EOL terminated line.  */
      while (curP != memP->endP
             && bufP != bufEndP
             && *curP != '\r' && *curP != '\n')
	*bufP++ = *curP++;
      if (curP == memP->endP || *curP == '\r' || *curP == '\n')
	{
	  /* Consume EOL.  */
	  if (curP != memP->endP && *curP == '\r')
	    ++curP;
	  if (curP != memP->endP && *curP == '\n')
	    ++curP;

	  /* Continuation line ? */
	  if (bufP != bufStartP && bufP[-1] == '\\')
	    {
	      --bufP;
	      gCurPObjP->lineNum++;
	      continue;
	    }

	  /* NUL terminate what we've just read as line.  */
	  if (bufP != bufEndP)
	    {
	      *bufP++ = '\0';
	      break;
	    }
	}
      Error_Abort ("Line too long");
    }
  gCurPObjP->lastLineSize = curP - memP->curP;
  memP->curP = curP;
  return false;
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
      case POType_eCachedFile:
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
 * \return A non-NULL pointer of canonicalized filename of the current parsing
 * object.
 */
const char *
FS_GetCurFileName (void)
{
  if (gCurPObjP == NULL)
    return gSourceFileName_Canon;
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
  assert (gPOStack[0].type == POType_eFile || gPOStack[0].type == POType_eCachedFile);
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
  assert (gPOStack[0].type == POType_eFile || gPOStack[0].type == POType_eCachedFile);
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
  assert (gPOStack[0].type == POType_eFile || gPOStack[0].type == POType_eCachedFile);
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

	  case POType_eCachedFile:
	    printf ("  - CachedFile: line num %d (if/while %d - %d): %s\n",
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
