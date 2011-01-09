/*
 * AS an assembler for ARM
 * Copyright (c) Andy Duplain, August 1992.
 * Copyright (c) 2004-2011 GCCSDK Developers
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
 * filestack.h
 */

#ifndef filestack_header_included
#define filestack_header_included

#include <stdbool.h>
#include <stdio.h>

#include "macros.h"
#include "whileif.h"

/* Linked list of filenames used during assembling.  They remain valid until
   the very end of execution.  */
typedef struct FileNameList
{
  struct FileNameList *nextP;
  char fileName[1]; /**< Canonicalised path in host's OS filename format.  */
} FileNameList;
extern FileNameList *gFileNameListP;

typedef struct
{
  FILE *fhandle;
} FilePObject;

#define PARSEOBJECT_STACK_SIZE (32)

/* The object to parse, either file or macro (latter basically assembler stored
   in memory.  */
typedef enum
{
  POType_eFile,
  POType_eMacro
} POType_e;
typedef struct
{
  POType_e type;
  union
    {
      FilePObject file;
      MacroPObject macro;
    } d;

  const char *name; /**< Current file name, or file name of current macro).
    Can be NULL. Prefer FS_GetCurFileName() to read. */
  int lineNum; /**< Current line number. Prefer FS_GetCurLineNumber() to read. */

  unsigned int whileIfCurDepth; /**< The current index in the while/if array for this object.  */
  unsigned int whileIfStartDepth; /**< The start index in the while/if array for this object.  */

  /**
   * Fills given buffer with one NUL terminated line.
   * \param bufP Buffer to fill.
   * \param bufSize Buffer size, the maximum which get filled.
   * \return true when failure or EOD, false otherwise.
   */
  bool (*GetLine)(char *bufP, size_t bufSize);
} PObject;

extern PObject gPOStack[PARSEOBJECT_STACK_SIZE];
extern PObject *gCurPObjP; /**< Current parsable object.  */

void FS_PushFilePObject (const char *fileName);
void FS_PopPObject (bool noCheck);

const char *FS_GetCurFileName (void);
int FS_GetCurLineNumber (void);

#endif
