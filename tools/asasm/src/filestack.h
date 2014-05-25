/*
 * AsAsm an assembler for ARM
 * Copyright (c) Andy Duplain, August 1992.
 * Copyright (c) 2004-2014 GCCSDK Developers
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

#ifndef filestack_header_included
#define filestack_header_included

#include <stdbool.h>
#include <stdio.h>

#include "phase.h"
#include "macros.h"

/* Linked list of filenames used during assembling.  They remain valid until
   the very end of execution.  */
typedef struct FileNameList
{
  struct FileNameList *nextP;
  char fileName[]; /**< Canonicalised path in host's OS filename format.  */
} FileNameList;
extern FileNameList *gFileNameListP;

typedef struct
{
  FILE *fhandle;
} PObject_File;

typedef struct
{
  const char *startP;
  const char *endP;
  const char *curP;
} PObject_CachedFile;

#define PARSEOBJECT_STACK_SIZE (32)

typedef enum
{
  POType_eFile,
  POType_eCachedFile,
  POType_eMacro
} POType_e;

/* Object to parse, either file, cached file or macro.  */
typedef struct
{
  POType_e type;
  union
    {
      PObject_File file;
      PObject_CachedFile memory;
      PObject_Macro macro;
    } d;

  const char *fileName; /**< Canonicalized current file name, or file name of
    current macro definition we're executing.
    Prefer FS_GetCurFileName() to read this field. */
  unsigned lineNum; /**< Current line number. Prefer FS_GetCurLineNumber()
    to read this field. */

  unsigned int whileIfCurDepth; /**< The current index in the while/if array for this object.  */
  unsigned int whileIfStartDepth; /**< The start index in the while/if array for this object.  */

  /**
   * Fills given buffer with one NUL terminated line.
   * \param bufP Buffer to fill.
   * \param bufSize Buffer size, the maximum which get filled.
   * \return true when failure or EOD, false otherwise.
   */
  bool (*GetLine)(char *bufP, size_t bufSize);

  size_t lastLineSize; /**< Size of the last line read by (*GetLine) *before*
    any variable substitution is done and before continuation character is
    taken into account.
    So this is *not* equal to what the parsable input is after
    Input_NextLine().  */
} PObject;

extern PObject gPOStack[PARSEOBJECT_STACK_SIZE];
extern PObject *gCurPObjP; /**< Current parsable object.  */

void FS_PrepareForPhase (Phase_e phase);

void FS_SetFileCacheSize (unsigned sizeMByte);

unsigned FS_GetMacroDepth (void);

const char *FS_StoreFileName (const char *fileNameP);

bool FS_PushFilePObject (const char *fileName);
void FS_PopPObject (bool noCheck);

const char *FS_GetCurFileName (void);
unsigned FS_GetCurLineNumber (void);

/* For {LINENUM}, {LINENUMUP} and {LINENUMUPPER} support.  */
unsigned FS_GetBuiltinVarLineNum (void);
unsigned FS_GetBuiltinVarLineNumUp (void);
unsigned FS_GetBuiltinVarLineNumUpper (void);

#ifdef DEBUG_FILESTACK
void ReportFSStack (const char *id);
#endif

#endif
