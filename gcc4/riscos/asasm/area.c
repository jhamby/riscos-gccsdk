/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
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
 * area.c
 */

#include "config.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "area.h"
#include "commands.h"
#include "error.h"
#include "expr.h"
#include "get.h"
#include "input.h"
#include "lex.h"
#include "main.h"
#include "symbol.h"

static void Area_Ensure (void);

/* Area name which will be used where there needs one to be created but has
   not been explicitely done by the user so far.  */
#define IMPLICIT_AREA_NAME "$$$$$$$"

#define DOUBLE_UP_TO (128*1024)
#define GROWSIZE      (16*1024)

Symbol *areaCurrentSymbol = NULL;
Symbol *areaEntrySymbol = NULL;
int areaEntryOffset;
Symbol *areaHeadSymbol = NULL;


static Area *
areaNew (Symbol *sym, int type)
{
  Area *res;
  if ((res = malloc (sizeof (Area))) == NULL)
    errorOutOfMem ();

  res->next = areaHeadSymbol;
  res->type = type;
  res->imagesize = 0;
  res->image = NULL;
  res->norelocs = 0;
  res->relocs = NULL;
  res->lits = NULL;

  areaHeadSymbol = sym;

  return res;
}


static bool
areaImage (Area *area, int newsize)
{
  unsigned char *newImage;
  if (area->imagesize)
    newImage = realloc (area->image, newsize);
  else
    newImage = malloc (newsize);
  if (!newImage)
    return false;

  area->imagesize = newsize;
  area->image = newImage;
  return true;
}


void
areaGrow (Area *area, int mingrow)
{
  /* When we want to grow an implicit area, it is time to give an error as
     this is not something we want to output.  */
  if (area->imagesize == 0)
    {
      assert (areaCurrentSymbol->area.info == area);
      if (!strcmp (areaCurrentSymbol->str, IMPLICIT_AREA_NAME))
	error (ErrorError, "No area defined");
    }

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
    errorAbort ("Internal areaGrow: out of memory, minsize = %d", mingrow);
}

void
areaInit (void)
{
  Area_Ensure ();
}

/**
 * Do an implicit LTORG at the end of all areas.
 */
void
areaFinish (void)
{
  for (areaCurrentSymbol = areaHeadSymbol;
       areaCurrentSymbol != NULL;
       areaCurrentSymbol = areaCurrentSymbol->area.info->next)
    litOrg (areaCurrentSymbol->area.info->lits);
}

/**
 * Implements ENTRY.
 */
bool
c_entry (void)
{
  if (areaCurrentSymbol)
    {
      if (areaEntrySymbol)
	error (ErrorError, "More than one ENTRY");
      else
	{
	  areaEntrySymbol = areaCurrentSymbol;
	  areaEntryOffset = areaCurrentSymbol->value.Data.Int.i;
	}
    }
  else
    errorAbort ("No area selected before ENTRY");
  return false;
}


/**
 * Implements ALIGN.
 */
bool
c_align (void)
{
  skipblanks ();
  int alignValue, offsetValue;
  if (Input_IsEolOrCommentStart ())
    {				/* no expression follows */
      alignValue = 1<<2;
      offsetValue = 0;
    }
  else
    {				/* an expression follows */
      /* Determine align value */
      Value value = exprBuildAndEval (ValueInt);
      if (value.Tag == ValueInt)
	{
	  alignValue = value.Data.Int.i;
	  if (alignValue <= 0 || (alignValue & (alignValue - 1)))
	    {
	      error (ErrorError, "ALIGN value is not a power of two");
	      alignValue = 1<<0;
	    }
	}
      else
	{
	  error (ErrorError, "Unrecognized ALIGN value");
	  alignValue = 1<<0;
	}

      /* Determine offset value */
      skipblanks ();
      if (Input_IsEolOrCommentStart ())
	offsetValue = 0;
      else if (inputGet () == ',')
	{
	  Value valueO = exprBuildAndEval (ValueInt);
	  if (valueO.Tag == ValueInt)
	    {
	      offsetValue = valueO.Data.Int.i;
	      if (offsetValue < 0)
		{
		  error (ErrorError, "ALIGN offset value is out-of-bounds");
		  offsetValue = 0;
		}
	    }
	  else
	    {
	      error (ErrorError, "Unrecognized ALIGN offset value");
	      offsetValue = 0;
	    }
	}
      else
	{
	  error (ErrorError, "Unrecognized ALIGN offset value");
	  offsetValue = 0;
	}
    }
  /* We have to align on alignValue + offsetValue */

  int unaligned = (offsetValue - areaCurrentSymbol->value.Data.Int.i) % alignValue;
  if (unaligned || offsetValue >= alignValue)
    {
      int bytesToStuff = (unaligned < 0) ? alignValue + unaligned : unaligned;

      bytesToStuff += (offsetValue / alignValue)*alignValue;

      if (AREA_NOSPACE (areaCurrentSymbol->area.info, areaCurrentSymbol->value.Data.Int.i + bytesToStuff))
	areaGrow (areaCurrentSymbol->area.info, bytesToStuff);

      for (; bytesToStuff; --bytesToStuff)
	areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.Data.Int.i++] = 0;
    }
  return false;
}


/**
 * Implements '%'.
 */
bool
c_reserve (void)
{
  Value value = exprBuildAndEval (ValueInt);
  if (value.Tag == ValueInt)
    {
      int i = areaCurrentSymbol->value.Data.Int.i;

      if (AREA_NOSPACE (areaCurrentSymbol->area.info, i + value.Data.Int.i))
	areaGrow (areaCurrentSymbol->area.info, value.Data.Int.i);

      areaCurrentSymbol->value.Data.Int.i += value.Data.Int.i;

      for (; i < areaCurrentSymbol->value.Data.Int.i; i++)
	areaCurrentSymbol->area.info->image[i] = 0;
    }
  else
    error (ErrorError, "Unresolved reserve not possible");
  return false;
}


/**
 * Ensures there is an active area (i.e. areaCurrentSymbol is non-NULL).
 * When the user didn't specify an area yet, there will be one created called
 * "$$$$$$$".
 */
static void
Area_Ensure (void)
{
  assert (areaCurrentSymbol == NULL);
  const Lex lex = lexTempLabel (IMPLICIT_AREA_NAME, sizeof (IMPLICIT_AREA_NAME)-1);
  Symbol *sym = symbolGet (&lex);
  if (sym->type & SYMBOL_DEFINED)
    error (ErrorError, "Redefinition of label to area %s", sym->str);
  else if (!(sym->type & SYMBOL_AREA))
    {
      sym->type = SYMBOL_AREA | SYMBOL_DECLARED;
      sym->value.Tag = ValueInt;
      sym->value.Data.Int.i = 0;
      sym->area.info = areaNew (sym, AREA_CODE | AREA_READONLY | AREA_INIT);
    }

  areaCurrentSymbol = sym;
}


bool
Area_IsImplicit (const Symbol *sym)
{
  return !strcmp (sym->str, IMPLICIT_AREA_NAME);
}


/**
 * Implements AREA.
 */
bool
c_area (void)
{
  Lex lex = lexGetId ();
  if (lex.tag != LexId)
    return false; /* FIXME: need for error msg here ? */
  Symbol *sym = symbolGet (&lex);
  int oldtype = 0;  
  if (sym->type & SYMBOL_DEFINED)
    error (ErrorError, "Redefinition of label to area %s", sym->str);
  else if (sym->type & SYMBOL_AREA)
    oldtype = sym->area.info->type;
  else
    {
      sym->type = SYMBOL_AREA | SYMBOL_DECLARED;
      sym->value.Tag = ValueInt;
      sym->value.Data.Int.i = 0;
      sym->area.info = areaNew (sym, 0);
    }
  skipblanks ();

  int newtype = 0;
  bool rel_specified = false, data_specified = false;
  int c;
  while ((c = inputGet ()) == ',')
    {
      Lex attribute = lexGetId ();
      if (!strncmp ("ABS", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  if (rel_specified)
	    error (ErrorError, "Conflicting area attributes ABS vs REL");
	  newtype |= AREA_ABS;
	}
      else if (!strncmp ("REL", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  if (newtype & AREA_ABS)
	    error (ErrorError, "Conflicting area attributes ABS vs REL");
	  rel_specified = true;
	}
      else if (!strncmp ("CODE", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  if (data_specified)
	    error (ErrorError, "Conflicting area attributes CODE vs DATA");
	  newtype |= AREA_CODE;
	}
      else if (!strncmp ("DATA", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  if (newtype & AREA_CODE)
	    error (ErrorError, "Conflicting area attributes CODE vs DATA");
	  data_specified = true;
	}
      else if (!strncmp ("COMDEF", attribute.Data.Id.str, attribute.Data.Id.len))
	newtype |= AREA_COMMONDEF;
      else if (!strncmp ("COMMON", attribute.Data.Id.str, attribute.Data.Id.len)
	       || !strncmp ("COMREF", attribute.Data.Id.str, attribute.Data.Id.len))
	newtype |= AREA_COMMONREF | AREA_UDATA;
      else if (!strncmp ("NOINIT", attribute.Data.Id.str, attribute.Data.Id.len))
	newtype |= AREA_UDATA;
      else if (!strncmp ("READONLY", attribute.Data.Id.str, attribute.Data.Id.len))
	newtype |= AREA_READONLY;
      else if (!strncmp ("PIC", attribute.Data.Id.str, attribute.Data.Id.len))
	newtype |= AREA_PIC;
      else if (!strncmp ("DEBUG", attribute.Data.Id.str, attribute.Data.Id.len))
	newtype |= AREA_DEBUG;
      else if (!strncmp ("REENTRANT", attribute.Data.Id.str, attribute.Data.Id.len))
	newtype |= AREA_REENTRANT;
      else if (!strncmp ("BASED", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  skipblanks ();
	  ARMWord reg = getCpuReg ();
	  newtype |= AREA_BASED | AREA_READONLY | (reg << 24);
	}
      else if (!strncmp ("LINKONCE", attribute.Data.Id.str, attribute.Data.Id.len))
	newtype |= AREA_LINKONCE;
      else if (!strncmp ("ALIGN", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  if (newtype & 0xFF)
	    error (ErrorError, "You can't specify ALIGN attribute more than once");
	  skipblanks ();
	  if (inputGet () != '=')
	    error (ErrorError, "Malformed ALIGN attribute specification");
	  skipblanks ();
	  Value value = exprBuildAndEval (ValueInt);
	  if (value.Tag == ValueInt)
	    {
	      if (value.Data.Int.i < 2 || value.Data.Int.i > 12)
		error (ErrorError, "ALIGN attribute value must be between 2 (incl) and 12 (incl)");
	      else
		newtype |= value.Data.Int.i;
	    }
	  else
	    error (ErrorError, "Unrecognized ALIGN attribute value");
	}
      else
	error (ErrorError, "Illegal area attribute %s", attribute.Data.Id.str);
      skipblanks ();
    }
  inputUnGet (c);

  /* Any alignment specified ? No, take default alignment (2) */
  if ((newtype & 0xFF) == 0)
    newtype |= AREA_INIT;

  /* AREA_COMMONDEF + AREA_COMMONREF => AREA_COMMONDEF */
  if (newtype & AREA_COMMONDEF)
    newtype &= ~AREA_COMMONREF;

  /* AREA_COMMONDEF + AREA_UDATA => AREA_COMMONREF */
  if ((newtype & AREA_COMMONDEF) && (newtype & AREA_UDATA))
    {
      newtype &= ~(AREA_COMMONDEF | AREA_UDATA);
      newtype |= AREA_COMMONREF;
    }

  /* Debug area ? Ignore code attribute */
  if (newtype & AREA_DEBUG)
    newtype &= ~AREA_CODE;

  if (newtype & AREA_CODE)
    {
      /* 32-bit and FPv3 flags should only be set on CODE areas.  */
      if (option_apcs_32bit)
	newtype |= AREA_32BITAPCS;
      if (option_apcs_fpv3)
	newtype |= AREA_EXTFPSET;
      if (option_apcs_softfloat)
	newtype |= AREA_SOFTFLOAT;
    }
  else if (newtype & (AREA_32BITAPCS | AREA_REENTRANT | AREA_EXTFPSET | AREA_NOSTACKCHECK))
    error (ErrorError, "Attribute REENTRANT may not be set for a DATA area");

  if ((newtype & AREA_READONLY) && (newtype & AREA_UDATA))
    error (ErrorError, "Attributes READONLY and NOINIT are mutually exclusive");

  if ((newtype & AREA_LINKONCE) && !(newtype & AREA_COMMONDEF))
    error (ErrorError, "Attribute LINKONCE must appear as part of a COMDEF");

  if (!(newtype & AREA_CODE) && (newtype & AREA_REENTRANT))
    error (ErrorError, "Attribute REENTRANT may not be set for DATA area");
  if ((newtype & AREA_CODE) && (newtype & AREA_BASED))
    error (ErrorError, "Attribute BASED may not be set for CODE area");

  if (newtype && oldtype && newtype != oldtype)
    error (ErrorError, "Changing attribute of area %s", sym->str);
  sym->area.info->type |= newtype;
  areaCurrentSymbol = sym;
  return false;
}
