/*
 * AS an assembler for ARM
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2000-2011 GCCSDK Developers
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

#include "area.h"
#include "asm.h"
#include "common.h"
#include "error.h"
#include "filestack.h"
#include "local.h"

#define kEmptyRoutineName "EmptyRName$$"

typedef struct RoutPos_t
{
  struct RoutPos_t *next;
  const char *id;
  const char *file;
  int lineno;
} RoutPos_t;

static unsigned int oRout_Null;
static RoutPos_t *oRout_List;
static RoutPos_t *oRout_ListEnd;

static Local_Label_t oLocal_LabelNum[32];

/* Parameters: AREA ptr, label number, instance number, routine name.  */
const char Local_IntLabelFormat[] = kIntLabelPrefix "Local$$%p$$%08i$$%i$$%s";

static void Local_ResetLabels (void);

void
Local_PrepareForPhase (ASM_Phase_e phase)
{
  switch (phase)
    {
      case eStartup:
	break;

      case ePassOne:
	Local_ResetLabels ();
	break;

      case ePassTwo:
	{
	  for (RoutPos_t *routCur = oRout_List; routCur != NULL; /* */)
	    {
	      RoutPos_t *routCurNext = routCur->next;
	      free ((void *)routCur->id);
	      free (routCur);
	      routCur = routCurNext;
	    }
	  oRout_List = NULL;
	  oRout_ListEnd = NULL;
	  oRout_Null = 0;
	  Local_ResetLabels ();
	}
	break;

      case eOutput:
	Local_ResetLabels ();
	break;
    }
}

Local_Label_t *
Local_GetLabel (unsigned num)
{
  unsigned i = num % (sizeof (oLocal_LabelNum) / sizeof (oLocal_LabelNum[0]));
  Local_Label_t *lblP;
  for (lblP = &oLocal_LabelNum[i];
       lblP->Num != num && lblP->NextP != NULL;
       lblP = lblP->NextP)
    /* */;
  if (lblP->Num == num)
    return lblP;
  lblP->NextP = malloc (sizeof (Local_Label_t));
  lblP->NextP->NextP = NULL;
  lblP->NextP->Num = num;
  lblP->NextP->Value = 0;
  return lblP->NextP;
}

static void
Local_ResetLabels (void)
{
  for (unsigned i = 0; i != sizeof (oLocal_LabelNum) / sizeof (oLocal_LabelNum[0]); ++i)
    {
      for (Local_Label_t *nextLblP = oLocal_LabelNum[i].NextP; nextLblP != NULL; /* */)
	{
	  Local_Label_t *nextNextLblP = nextLblP->NextP;
	  free (nextLblP);
	  nextLblP = nextNextLblP;
	}
      memset (&oLocal_LabelNum[i], 0, sizeof (oLocal_LabelNum[0]));
    }
}

const char *
Local_GetCurROUTId (void)
{
  return oRout_ListEnd ? oRout_ListEnd->id : kEmptyRoutineName "0";
}

/**
 * Implements ROUT.
 */
bool
c_rout (const Lex *label)
{
  Local_ResetLabels ();

  char *newROUTId;
  if (label->tag == LexId)
    {
      ASM_DefineLabel (label, areaCurrentSymbol->area.info->curIdx);
      if (Local_ROUTIsEmpty (label->Data.Id.str))
	{
	  error (ErrorError, "Illegal routine name");
	  return false;
	}
      newROUTId = strndup (label->Data.Id.str, label->Data.Id.len);
    }
  else
    {
      newROUTId = malloc (sizeof (kEmptyRoutineName)-1 + 10);
      if (newROUTId != NULL)
        sprintf (newROUTId, kEmptyRoutineName "%i", ++oRout_Null);
    }
  RoutPos_t *p = malloc (sizeof (RoutPos_t));
  if (newROUTId == NULL || p == NULL)
    errorOutOfMem ();

  if (oRout_ListEnd)
    oRout_ListEnd->next = p;
  oRout_ListEnd = p;
  if (!oRout_List)
    oRout_List = p;

  p->next = NULL;
  p->id = newROUTId;
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
  for (const RoutPos_t *p = oRout_List; p; p = p->next)
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

#ifdef DEBUG
void
Local_DumpAll (void)
{
  for (const RoutPos_t *p = oRout_List; p != NULL; p = p->next)
    {
      printf ("%s : %s @ line %d\n", p->id, p->file, p->lineno);
    }
}
#endif
