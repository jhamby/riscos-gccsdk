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

#define DOUBLE_UP_TO (128*1024)
#define GROWSIZE      (16*1024)

Symbol *areaCurrentSymbol = NULL;
Symbol *areaEntrySymbol = NULL;
int areaEntryOffset;
Symbol *areaHeadSymbol = NULL;


void
areaError (void)
{
  errorAbort ("No area defined");
}


static Area *
areaNew (int type)
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
  return res;
}


static bool
areaImage (Area * area, int newsize)
{
  unsigned char *newImage;
  if (area->imagesize)
    newImage = realloc (area->image, newsize);
  else
    newImage = malloc (newsize);
  if (newImage)
    {
      area->imagesize = newsize;
      area->image = newImage;
      return true;
    }
  else
    return false;
}


void
areaGrow (Area *area, int mingrow)
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
    errorAbort ("Internal areaGrow: out of memory, minsize = %d", mingrow);
}

void
areaInit (void)
{
  areaCurrentSymbol = NULL;
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

void
c_entry (void)
{
  if (areaCurrentSymbol)
    {
      if (areaEntrySymbol)
	error (ErrorError, "More than one ENTRY");
      else
	{
	  areaEntrySymbol = areaCurrentSymbol;
	  areaEntryOffset = areaCurrentSymbol->value.ValueInt.i;
	}
    }
  else
    errorAbort ("No area selected before ENTRY");
}


void
c_align (void)
{
  int alignValue, offsetValue, unaligned;

  if (!areaCurrentSymbol)
    {
      areaError ();
      return;
    }

  skipblanks ();
  if (inputComment ())
    {				/* no expression follows */
      alignValue = 1<<2;
      offsetValue = 0;
    }
  else
    {				/* an expression follows */
      /* Determine align value */
      exprBuild ();
      Value value = exprEval (ValueInt);
      if (value.Tag == ValueInt)
	{
	  alignValue = value.ValueInt.i;
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
      if (inputComment ())
	offsetValue = 0;
      else if (inputGet () == ',')
	{
	  exprBuild ();
	  Value valueO = exprEval (ValueInt);
	  if (valueO.Tag == ValueInt)
	    {
	      offsetValue = valueO.ValueInt.i;
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

  unaligned = (offsetValue - areaCurrentSymbol->value.ValueInt.i) % alignValue;
  if (unaligned || offsetValue >= alignValue)
    {
      int bytesToStuff = (unaligned < 0) ? alignValue + unaligned : unaligned;

      bytesToStuff += (offsetValue / alignValue)*alignValue;

      if (AREA_NOSPACE (areaCurrentSymbol->area.info, areaCurrentSymbol->value.ValueInt.i + bytesToStuff))
	areaGrow (areaCurrentSymbol->area.info, bytesToStuff);

      for (; bytesToStuff; --bytesToStuff)
	areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.ValueInt.i++] = 0;
    }
}


void
c_reserve (void)
{
  if (!areaCurrentSymbol)
    {
      areaError ();
      return;
    }

  exprBuild ();
  Value value = exprEval (ValueInt);
  if (value.Tag == ValueInt)
    {
      int i = areaCurrentSymbol->value.ValueInt.i;

      if (AREA_NOSPACE (areaCurrentSymbol->area.info, i + value.ValueInt.i))
	areaGrow (areaCurrentSymbol->area.info, value.ValueInt.i);

      areaCurrentSymbol->value.ValueInt.i += value.ValueInt.i;

      for (; i < areaCurrentSymbol->value.ValueInt.i; i++)
	areaCurrentSymbol->area.info->image[i] = 0;
    }
  else
    error (ErrorError, "Unresolved reserve not possible");
}


void
c_area (void)
{
  int oldtype = 0;
  int newtype = 0;
  int rel_specified = 0, data_specified = 0;
  
  Lex lex = lexGetId ();
  Symbol *sym = symbolGet (&lex);
  if (sym->type & SYMBOL_DEFINED)
    error (ErrorError, "Redefinition of label to area %s", sym->str);
  else if (sym->type & SYMBOL_AREA)
    oldtype = sym->area.info->type;
  else
    {
      sym->type = SYMBOL_AREA | SYMBOL_DECLARED;
      sym->value.Tag = ValueInt;
      sym->value.ValueInt.i = 0;
      sym->area.info = areaNew (0);
      areaHeadSymbol = sym;
    }
  skipblanks ();
  int c;
  while ((c = inputGet ()) == ',')
    {
      Lex attribute = lexGetId ();
      if (!strncmp ("ABS", attribute.LexId.str, attribute.LexId.len))
	{
	  if (rel_specified)
	    error (ErrorError, "Conflicting area attributes ABS vs REL");
	  newtype |= AREA_ABS;
	}
      else if (!strncmp ("REL", attribute.LexId.str, attribute.LexId.len))
	{
	  if (newtype & AREA_ABS)
	    error (ErrorError, "Conflicting area attributes ABS vs REL");
	  rel_specified = 1;
	}
      else if (!strncmp ("CODE", attribute.LexId.str, attribute.LexId.len))
	{
	  if (data_specified)
	    error (ErrorError, "Conflicting area attributes CODE vs DATA");
	  newtype |= AREA_CODE;
	}
      else if (!strncmp ("DATA", attribute.LexId.str, attribute.LexId.len))
	{
	  if (newtype & AREA_CODE)
	    error (ErrorError, "Conflicting area attributes CODE vs DATA");
	  data_specified = 1;
	}
      else if (!strncmp ("COMDEF", attribute.LexId.str, attribute.LexId.len))
	newtype |= AREA_COMMONDEF;
      else if (!strncmp ("COMMON", attribute.LexId.str, attribute.LexId.len)
	       || !strncmp ("COMREF", attribute.LexId.str, attribute.LexId.len))
	newtype |= AREA_COMMONREF | AREA_UDATA;
      else if (!strncmp ("NOINIT", attribute.LexId.str, attribute.LexId.len))
	newtype |= AREA_UDATA;
      else if (!strncmp ("READONLY", attribute.LexId.str, attribute.LexId.len))
	newtype |= AREA_READONLY;
      else if (!strncmp ("PIC", attribute.LexId.str, attribute.LexId.len))
	newtype |= AREA_PIC;
      else if (!strncmp ("DEBUG", attribute.LexId.str, attribute.LexId.len))
	newtype |= AREA_DEBUG;
      else if (!strncmp ("REENTRANT", attribute.LexId.str, attribute.LexId.len))
	newtype |= AREA_REENTRANT;
      else if (!strncmp ("BASED", attribute.LexId.str, attribute.LexId.len))
	{
	  ARMWord reg;
	  skipblanks ();
	  reg = getCpuReg ();
	  newtype |= AREA_BASED | AREA_READONLY | (reg << 24);
	}
      else if (!strncmp ("LINKONCE", attribute.LexId.str, attribute.LexId.len))
	newtype |= AREA_LINKONCE;
      else if (!strncmp ("ALIGN", attribute.LexId.str, attribute.LexId.len))
	{
	  if (newtype & 0xFF)
	    error (ErrorError, "You can't specify ALIGN attribute more than once");
	  skipblanks ();
	  if (inputGet () != '=')
	    error (ErrorError, "Malformed ALIGN attribute specification");
	  skipblanks ();
	  exprBuild ();
	  Value value = exprEval (ValueInt);
	  if (value.Tag == ValueInt)
	    {
	      if (value.ValueInt.i < 2 || value.ValueInt.i > 12)
		error (ErrorError, "ALIGN attribute value must be between 2 (incl) and 12 (incl)");
	      else
		newtype |= value.ValueInt.i;
	    }
	  else
	    error (ErrorError, "Unrecognized ALIGN attribute value");
	}
      else
	error (ErrorError, "Illegal area attribute %s", attribute.LexId.str);
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
    {
      error (ErrorError, "Attribute REENTRANT may not be set for a DATA area");
    }

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
}
