/*
 * area.c
 * Copyright © 1992 Niklas Röjemo
 */

#include "sdk-config.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "lex.h"
#include "hash.h"
#include "symbol.h"
#include "commands.h"
#include "error.h"
#include "input.h"
#include "expr.h"
#include "area.h"
#include "get.h"

#define DOUBLE_UP_TO 128*1024
#define GROWSIZE      16*1024

Symbol *areaCurrent = 0;
Symbol *areaEntry = 0;
int areaEntryOffset;

Symbol *areaHead = 0;

extern int apcs_32bit, apcs_softfloat, apcs_fpv3;

void
areaError (void)
{
  static int reported = 0;
  if (!reported)
    {
      reported = 1;
      error (ErrorSerious, TRUE, "No area defined");
    }
}

static Area *
areaNew (int type)
{
  Area *res = malloc (sizeof (Area));
  if (res)
    {
      res->next = areaHead;
      res->type = type | AREA_INIT;
      res->imagesize = 0;
      res->image = 0;
      res->norelocs = 0;
      res->relocs = 0;
      res->lits = 0;
    }
  else
    errorOutOfMem ("areaNew");
  return res;
}

static BOOL
areaImage (Area * area, int newsize)
{
  unsigned char *new;
  if (area->imagesize)
    new = realloc (area->image, newsize);
  else
    new = malloc (newsize);
  if (new)
    {
      area->imagesize = newsize;
      area->image = new;
      return TRUE;
    }
  else
    return FALSE;
}


void
areaGrow (Area * area, int mingrow)
{
  int inc;
  if (area->imagesize && area->imagesize < DOUBLE_UP_TO)
    inc = area->imagesize;
  else
    inc = GROWSIZE;
  if (inc < mingrow)
    inc = mingrow;
  while (inc > mingrow && !areaImage (area, area->imagesize + inc))
    inc /= 2;
  if (inc <= mingrow && !areaImage (area, area->imagesize + mingrow))
    error (ErrorSerious, FALSE,
	   "Internal areaGrow: out of memory, minsize = %d", mingrow);
}

void
areaInit (void)
{
  areaCurrent = 0;
}

void
areaFinish (void)		/* insert ltorg at end of all areas */
{
  Symbol *ap;
  for (ap = areaHead; ap; ap = ap->area.info->next)
    {
      areaCurrent = ap;
      litOrg (ap->area.info->lits);
    }
}

void
c_entry (void)
{
  if (areaCurrent)
    {
      if (areaEntry)
	error (ErrorError, FALSE, "More than one ENTRY");
      else
	{
	  areaEntry = areaCurrent;
	  areaEntryOffset = areaCurrent->value.ValueInt.i;
	}
    }
  else
    error (ErrorError, FALSE, "No area selected before ENTRY");
}


void
c_align (void)
{
  Value value;
  skipblanks ();
  if (!areaCurrent)
    {
      areaError ();
      return;
    }
  if (inputComment ())
    {				/* no expression follows */
      if (areaCurrent->area.info->image)
	{
	  while (areaCurrent->value.ValueInt.i & 3)
	    areaCurrent->area.info->image[areaCurrent->value.ValueInt.i++] = 0;
	}
      else
	areaCurrent->value.ValueInt.i = (areaCurrent->value.ValueInt.i + 3) & ~3;
    }
  else
    {				/* an expression follows */
      exprBuild ();
      value = exprEval (ValueInt);
      if (value.Tag.t == ValueInt)
	{
	  if (areaCurrent->area.info->image)
	    {
	      while (areaCurrent->value.ValueInt.i % value.ValueInt.i)
		areaCurrent->area.info->image[areaCurrent->value.ValueInt.i++] = 0;
	    }
	  else
	    {
	      areaCurrent->value.ValueInt.i /= value.ValueInt.i;
	      areaCurrent->value.ValueInt.i++;
	      areaCurrent->value.ValueInt.i *= value.ValueInt.i;
	    }
	}
      else
	error (ErrorError, TRUE, "Unresolved align not possible");
    }
}

void
c_reserve (void)
{
  Value value;
  exprBuild ();
  value = exprEval (ValueInt);
  switch (value.Tag.t)
    {
    case ValueInt:
      if (!areaCurrent)
	areaError ();
      else
	{
	  int i = areaCurrent->value.ValueInt.i;

	  if (AREA_NOSPACE (areaCurrent->area.info, i + value.ValueInt.i))
	    areaGrow (areaCurrent->area.info, value.ValueInt.i);

	  areaCurrent->value.ValueInt.i += value.ValueInt.i;

	  if (areaCurrent->area.info->image)
	    for (; i < areaCurrent->value.ValueInt.i; i++)
	      areaCurrent->area.info->image[i] = 0;
	}
      break;
    default:
      error (ErrorError, TRUE, "Unresolved reserve not possible");
    }
}


void
c_area (void)
{
  Symbol *sym;
  int oldtype = 0;
  int newtype = 0;
  int c;

  sym = symbolGet (lexGetId ());
  if (sym->type & SYMBOL_DEFINED)
    error (ErrorError, TRUE, "Redefinition of label to area %s", sym->str);
  else if (sym->type & SYMBOL_AREA)
    oldtype = sym->area.info->type;
  else
    {
      sym->type = SYMBOL_AREA;
      sym->declared = 1;
      sym->value.Tag.t = ValueInt;
      sym->value.ValueInt.i = 0;
      sym->area.info = areaNew (0);
      areaHead = sym;
    }
  skipblanks ();
  while ((c = inputGet ()) == ',')
    {
      Lex attribute = lexGetId ();
      if (!strncmp ("CODE", attribute.LexId.str, attribute.LexId.len))
	newtype |= AREA_CODE | AREA_INIT;
      else if (!strncmp ("DATA", attribute.LexId.str, attribute.LexId.len))
	newtype |= AREA_DATA | AREA_INIT;
      else if (!strncmp ("NOINIT", attribute.LexId.str, attribute.LexId.len))
	newtype |= AREA_UDATA | AREA_INIT;
      else if (!strncmp ("READONLY", attribute.LexId.str, attribute.LexId.len))
	newtype |= AREA_READONLY | AREA_INIT;
      else if (!strncmp ("COMMON", attribute.LexId.str, attribute.LexId.len)
	   || !strncmp ("COMDEF", attribute.LexId.str, attribute.LexId.len))
	newtype |= AREA_COMMONDEF | AREA_COMMONREF | AREA_UDATA | AREA_INIT;
      else if (!strncmp ("COMREF", attribute.LexId.str, attribute.LexId.len))
	newtype |= AREA_COMMONREF | AREA_UDATA | AREA_INIT;
      else if (!strncmp ("DEBUG", attribute.LexId.str, attribute.LexId.len))
	newtype |= AREA_DEBUG | AREA_READONLY | AREA_INIT;
      else if (!strncmp ("PIC", attribute.LexId.str, attribute.LexId.len))
	newtype |= AREA_PIC | AREA_INIT;
      else if (!strncmp ("REENTRANT", attribute.LexId.str, attribute.LexId.len))
	newtype |= AREA_REENTRANT | AREA_INIT;
      else if (!strncmp ("BASED", attribute.LexId.str, attribute.LexId.len))
	{
	  WORD reg;

	  skipblanks ();
	  reg = getCpuReg ();
	  newtype |= AREA_BASED | AREA_READONLY | AREA_INIT | (reg << 24);
	}
      else
	error (ErrorError, TRUE, "Illegal area attribute %s", attribute.LexId.str);
      skipblanks ();
    }
  inputUnGet (c);

  if (newtype & AREA_CODE)
    {
      /* 32-bit and FPv3 flags should only be set on CODE areas.  */
      if (apcs_32bit)
	newtype |= AREA_32BITAPCS;
      if (apcs_fpv3)
	newtype |= AREA_EXTFPSET;
    }

  if ((newtype & AREA_READONLY) && (newtype & AREA_UDATA))
    error (ErrorError, TRUE, "Attributes READONLY and NOINIT are mutually exclusive");

  if (!(newtype & AREA_CODE))
    {
      if (newtype & AREA_REENTRANT)
	error (ErrorError, TRUE, "Attribute CODE must be set to use REENTRANT");
    }
  if (newtype & AREA_CODE)
    {
      if (newtype & AREA_BASED)
	error (ErrorError, TRUE, "Attribute CODE must be unset to use BASED");
    }

  if (newtype && oldtype && newtype != oldtype)
    error (ErrorError, TRUE, "Changing attribute of area %s", sym->str);
  sym->area.info->type |= newtype;
  areaCurrent = sym;
}
