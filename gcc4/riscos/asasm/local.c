/*
 * AS an assembler for ARM
 * Copyright (c) 1997 Darren Salt
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
 * local.c
 */

#include "config.h"
#include <string.h>
#include <stdlib.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "asm.h"
#include "error.h"
#include "filestack.h"
#include "local.h"

#define kEmptyRoutineName "EmptyRName$$"

typedef struct routPos
{
  struct routPos *next;
  const char *id;
  const char *file;
  int lineno;
} routPos;

static unsigned int rout_null = 0;
static routPos *routList;
static routPos *routListEnd;

int Local_ROUTLblNo[100] = {0};
const char *Local_CurROUTId = kEmptyRoutineName "0";

#define kIntLabelPrefix "$$AsAsm$$Local$$"
/* Parameters: AREA ptr, 0 - 99 label digit, instance number, routine name.  */
const char Local_IntLabelFormat[] = kIntLabelPrefix "%p$$%02i$$%i$$%s";


/**
 * Implements ROUT.
 */
bool
c_rout (const Lex *label)
{
  memset (Local_ROUTLblNo, 0, sizeof (Local_ROUTLblNo));

  if (label->tag == LexId)
    {
      ASM_DefineLabel (label, 0); /* FIXME: should we really define this as a label ? */
      if (Local_ROUTIsEmpty (label->Data.Id.str))
	{
	  error (ErrorError, "Illegal routine name");
	  return false;
	}
      Local_CurROUTId = strndup (label->Data.Id.str, label->Data.Id.len);
    }
  else
    {
      char *new_rout_id = malloc (sizeof (kEmptyRoutineName)-1 + 10);
      if (new_rout_id)
	{
	  sprintf (new_rout_id, kEmptyRoutineName "%i", ++rout_null);
	  Local_CurROUTId = new_rout_id;
	}
    }
  routPos *p = malloc (sizeof (routPos));
  if (Local_CurROUTId == NULL || p == NULL)
    errorOutOfMem ();

  if (routListEnd)
    routListEnd->next = p;
  routListEnd = p;
  if (!routList)
    routList = p;

  p->id = Local_CurROUTId;
  p->file = FS_GetCurFileName ();
  p->lineno = FS_GetCurLineNumber ();
  return false;
}


bool
Local_ROUTIsEmpty (const char *routName)
{
  return !memcmp (routName, kEmptyRoutineName, sizeof (kEmptyRoutineName)-1);
}


bool
Local_IsLocalLabel (const char *s)
{
  return !memcmp (s, Local_IntLabelFormat, sizeof (kIntLabelPrefix)-1);
}


void
Local_FindROUT (const char *rout, const char **file, int *lineno)
{
  for (const routPos *p = routList; p; p = p->next)
    {
      if (!strcmp (p->id, rout))
	{
	  *file = p->file;
	  *lineno = p->lineno;
	  return;
	}
    }
  *file = NULL;
  *lineno = 0;
}
