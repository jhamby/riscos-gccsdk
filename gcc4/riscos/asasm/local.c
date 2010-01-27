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
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "asm.h"
#include "error.h"
#include "filestack.h"
#include "input.h"
#include "local.h"
#include "main.h"
#include "os.h"
#include "variables.h"

typedef struct localPos
  {
    struct localPos *next;
    int local;
    const char *file;
    int lineno;
  }
localPos;


typedef struct routPos
  {
    struct routPos *next;
    const char *id;
    const char *file;
    int lineno;
  }
routPos;


int rout_lblno[100] = {0};
const char *rout_id = "Local$$0";
int rout_null = 0;
static routPos *routList;
static routPos *routListEnd;

const char localFormat[] = "|Local$$%p$$%02i$$%i$$%s|";

int localCurrent = 0;
static localPos *localList;
static localPos *localListEnd;


void
c_rout (Lex *label)
{
  routPos *p = malloc (sizeof (localPos));
  memset (rout_lblno, 0, sizeof (rout_lblno));
  if (label->tag == LexId)
    {
      asm_label (label);
      rout_id = strndup (label->LexId.str, label->LexId.len);
    }
  else
    {
      char *new_rout_id = malloc (16);
      if (new_rout_id)
	{
	  sprintf (new_rout_id, "Local$$%i", rout_null++);
	  rout_id = new_rout_id;
	}
    }
  if (rout_id == NULL || p == NULL)
    errorOutOfMem ();
  if (routListEnd)
    routListEnd->next = p;
  routListEnd = p;
  if (!routList)
    routList = p;
  p->id = rout_id;
  p->file = FS_GetCurFileName ();
  p->lineno = FS_GetCurLineNumber ();
}


void
c_local (Lex *label)
{
  if (label && label->tag != LexNone)
    error (ErrorWarning, "Label not allowed here - ignoring");
  localCurrent++;
  localPos *p;
  if ((p = malloc (sizeof (localPos))) == NULL)
    errorOutOfMem ();
  if (localListEnd)
    localListEnd->next = p;
  localListEnd = p;
  if (!localList)
    localList = p;
  p->local = localCurrent;
  p->file = FS_GetCurFileName ();
  p->lineno = FS_GetCurLineNumber ();
}


int
localTest (const char *s)
{
  return !memcmp (s, localFormat + 1, sizeof ("Local$$")-1);
}


void
localMunge (Lex *label)
{
  int i;
  if (!option_local)
    return;
  if (label->LexId.str[i = label->LexId.len - 2] == '$'
      && (label->LexId.str[i + 1] == 'L' || label->LexId.str[i + 1] == 'l'))
    {
      char id[1024];
      sprintf (id, "Local$$%i$$", localCurrent);
      id[strlen (id) + label->LexId.len - 2] = 0;
      memcpy (id + strlen (id), label->LexId.str, label->LexId.len - 2);

      label->LexId.str = strdup (id);
      label->LexId.len = strlen (id);
      if (!label->LexId.str)
	errorOutOfMem ();
    }
}


void
localFindLocal (int local, const char **file, int *lineno)
{
  localPos *p;
  for (p = localList; p && p->local != local; p = p->next)
    /* */;
  if (p)
    {
      *file = p->file;
      *lineno = p->lineno;
    }
  else
    {
      *file = NULL;
      *lineno = 0;
    }
}


void
localFindRout (const char *rout, const char **file, int *lineno)
{
  routPos *p;
  for (p = routList; p && strcmp (p->id, rout); p = p->next)
    /* */;
  if (p)
    {
      *file = p->file;
      *lineno = p->lineno;
    }
  else
    {
      *file = NULL;
      *lineno = 0;
    }
}
