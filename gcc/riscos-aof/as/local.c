/*
 * local.c
 * Copyright © 1997 Darren Salt
 */
#include "sdk-config.h"
#include <string.h>
#include <stdlib.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "local.h"
#include "error.h"
#include "asm.h"
#include "input.h"
#include "variables.h"
#include "os.h"

typedef struct localPos
  {
    struct localPos *next;
    int local;
    const char *file;
    long int lineno;
  }
localPos;


typedef struct routPos
  {
    struct routPos *next;
    const char *id;
    const char *file;
    long int lineno;
  }
routPos;


int rout_lblno[100] =
{0};
const char *rout_id = "Local$$0";
int rout_null = 0;
static routPos
* routList = 0, *routListEnd = 0;

const char localFormat[] = "|Local$$%08x$$%02i$$%i$$%s|";

int localCurrent;		/* initial value doesn't matter */
static localPos
* localList = 0, *localListEnd = 0;


void 
c_rout (Lex * label)
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
  if (!rout_id)
    errorOutOfMem ("c_rout");
  if (p)
    {
      if (routListEnd)
	routListEnd->next = p;
      routListEnd = p;
      if (!routList)
	routList = p;
      p->id = rout_id;
      p->file = inputName;
      p->lineno = inputLineNo;
    }
}


void 
c_local (Lex * label)
{
  localPos *p = malloc (sizeof (localPos));

  if (label && label->tag != LexNone)
    error (ErrorWarning, TRUE, "Label not allowed here - ignoring");
  localCurrent++;
  if (p)
    {
      if (localListEnd)
	localListEnd->next = p;
      localListEnd = p;
      if (!localList)
	localList = p;
      p->local = localCurrent;
      p->file = inputName;
      p->lineno = inputLineNo;
    }
}


int 
localTest (const char *s)
{
  return !memcmp (s, localFormat + 1, 7);
}


extern int local;		/* in main.c */

void 
localMunge (Lex * label)
{
  register int i;
  if (!local)
    return;
  if (label->LexId.str[i = label->LexId.len - 2] == '$' &&
      (label->LexId.str[i + 1] == 'L' || label->LexId.str[i + 1] == 'l'))
    {
      char id[1024];
      sprintf (id, "Local$$%i$$", localCurrent);
      id[strlen (id) + label->LexId.len - 2] = 0;
      memcpy (id + strlen (id), label->LexId.str, label->LexId.len - 2);

      label->LexId.str = strdup (id);
      label->LexId.len = strlen (id);
      if (!label->LexId.str)
	{
	  errorOutOfMem ("lexGetId");
	  label->tag = LexNone;
	}
    }
}


void 
localFindLocal (int local, const char **file, long int *lineno)
{
  localPos *p = localList;
  while (p && p->local != local)
    p = p->next;
  if (p)
    {
      *file = p->file;
      *lineno = p->lineno;
    }
  else
    {
      *file = 0;
      *lineno = 0;
    }
}


void 
localFindRout (const char *rout, const char **file, long int *lineno)
{
  routPos *p = routList;
  while (p && strcmp (p->id, rout))
    p = p->next;
  if (p)
    {
      *file = p->file;
      *lineno = p->lineno;
    }
  else
    {
      *file = 0;
      *lineno = 0;
    }
}
