/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
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
 * area.c
 */

#include "config.h"

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "area.h"
#include "asm.h"
#include "commands.h"
#include "error.h"
#include "expr.h"
#include "filestack.h"
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
static Area_eEntryType oArea_CurrentEntryType = eInvalid;
Symbol *areaEntrySymbol = NULL;
static const char *oArea_EntrySymbolFile = NULL;
static int oArea_EntrySymbolLineNum = 0;
int areaEntryOffset = 0;
Symbol *areaHeadSymbol = NULL;

/* FIXME: gArea_Require8 & gArea_Preserve8(Guessed) needs to be written in ELF output.  */
bool gArea_Require8 = false; /* Absense of REQUIRE8 => {FALSE} */
Preserve8_eValue gArea_Preserve8 = ePreserve8_Guess;
bool gArea_Preserve8Guessed = true;

static struct PendingORG
{
  bool isValid; /** true when we have a pending ORG statement.  */
  const char *file; /** Non-NULL for pending ORG statement and points to filename of that ORG.  */
  int line; /** Linenumber of pending ORG.  */
  uint32_t value; /** Pending ORG value.  */
} oPendingORG;

static void
Area_ResetPrivateVars (void)
{
  areaCurrentSymbol = NULL;
  oArea_CurrentEntryType = eInvalid;
  areaEntrySymbol = NULL;
  oArea_EntrySymbolFile = NULL;
  oArea_EntrySymbolLineNum = 0;
  areaEntryOffset = 0;
  /* areaHeadSymbol = NULL; */

  gArea_Require8 = false; /* Absense of REQUIRE8 => {FALSE} */
  gArea_Preserve8 = ePreserve8_Guess;
  gArea_Preserve8Guessed = true;

  oPendingORG.isValid = false;
}

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

  res->curIdx = 0;
  res->maxIdx = 0;

  res->relocQueue = NULL;
  res->norelocs = 0;
  res->relocs = NULL;

  res->litPool = NULL;

  areaHeadSymbol = sym;

  return res;
}


static bool
areaImage (Area *area, size_t newsize)
{
  uint8_t *newImage;
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


/**
 * Ensures the current area has at least \t mingrow bytes free.
 */
void
Area_EnsureExtraSize (size_t mingrow)
{
  if (areaCurrentSymbol->area.info->curIdx + mingrow <= areaCurrentSymbol->area.info->imagesize)
    return;

  assert (gASM_Phase == ePassOne);
  
  /* When we want to grow an implicit area, it is time to give an error as
     this is not something we want to output.  */
  if (areaCurrentSymbol->area.info->imagesize == 0
      && Area_IsImplicit (areaCurrentSymbol))
    error (ErrorError, "No area defined");

  size_t inc;
  if (areaCurrentSymbol->area.info->imagesize && areaCurrentSymbol->area.info->imagesize < DOUBLE_UP_TO)
    inc = areaCurrentSymbol->area.info->imagesize;
  else
    inc = GROWSIZE;
  if (inc < mingrow)
    inc = mingrow;
  while (inc > mingrow && !areaImage (areaCurrentSymbol->area.info, areaCurrentSymbol->area.info->imagesize + inc))
    inc /= 2;
  if (inc <= mingrow && !areaImage (areaCurrentSymbol->area.info, areaCurrentSymbol->area.info->imagesize + mingrow))
    errorAbort ("Area_EnsureExtraSize(): out of memory, minsize = %zd", mingrow);
}

/**
 * Area phase preparation.
 */
void
Area_PrepareForPhase (ASM_Phase_e phase)
{
  switch (phase)
    {
      case ePassOne:
	Area_ResetPrivateVars ();
	Area_Ensure ();
	break;

      case ePassTwo:
	{
	  if (oPendingORG.isValid)
	    errorLine (oPendingORG.file, oPendingORG.line,
		       ErrorWarning, "Unused ORG statement");

	  /* Do an implicit LTORG at the end of all areas.  */
	  for (areaCurrentSymbol = areaHeadSymbol;
	       areaCurrentSymbol != NULL;
	       areaCurrentSymbol = areaCurrentSymbol->area.info->next)
	    {
	      Lit_DumpPool ();
	      areaCurrentSymbol->area.info->maxIdx = areaCurrentSymbol->area.info->curIdx;
	      areaCurrentSymbol->area.info->curIdx = 0;
	    }

	  Area_ResetPrivateVars ();
	  Area_Ensure ();
	  break;
	}

      case eOutput:
	{
	  if (oPendingORG.isValid)
	    errorLine (oPendingORG.file, oPendingORG.line,
		       ErrorWarning, "Unused ORG statement");

	  /* Do an implicit LTORG at the end of all areas.  */
	  for (areaCurrentSymbol = areaHeadSymbol;
	       areaCurrentSymbol != NULL;
	       areaCurrentSymbol = areaCurrentSymbol->area.info->next)
	    {
	      Lit_DumpPool ();
	      assert (areaCurrentSymbol->area.info->curIdx == areaCurrentSymbol->area.info->maxIdx);
	    }

	  /* Revert sort the area's so they become listed chronologically.  */
	  Symbol *aSymP = areaHeadSymbol;
	  assert (aSymP != NULL); /* There is always at least one area.  */
	  for (Symbol *nextSymP = aSymP->area.info->next; nextSymP != NULL; /* */)
	    {
	      Symbol *nextNextSymP = nextSymP->area.info->next;

	      nextSymP->area.info->next = aSymP;
	      aSymP = nextSymP;

	      nextSymP = nextNextSymP;
	    }
	  areaHeadSymbol->area.info->next = NULL;
	  areaHeadSymbol = aSymP;
	  break;
	}

      default:
	break;
    }
}


/**
 * Implements ENTRY.
 */
bool
c_entry (void)
{
  if (Area_IsImplicit (areaCurrentSymbol))
    error (ErrorError, "No area selected before ENTRY");
  else
    {
      if (areaEntrySymbol)
	{
	  error (ErrorError, "More than one ENTRY");
	  errorLine (oArea_EntrySymbolFile, oArea_EntrySymbolLineNum,
	             ErrorError, "note: Previous ENTRY was here"); 
	}
      else
	{
	  areaEntrySymbol = areaCurrentSymbol;
	  areaEntryOffset = areaCurrentSymbol->area.info->curIdx;
	  oArea_EntrySymbolFile = FS_GetCurFileName ();
	  oArea_EntrySymbolLineNum = FS_GetCurLineNumber ();
	}
    }

  return false;
}


/**
 * Implements ALIGN [<power-of-2> [, <offset>]]
 */
bool
c_align (void)
{
  skipblanks ();
  uint32_t alignValue, offsetValue;
  if (Input_IsEolOrCommentStart ())
    {
      alignValue = 1<<2;
      offsetValue = 0;
    }
  else
    {
      /* Determine align value */
      const Value *value = exprBuildAndEval (ValueInt);
      if (value->Tag == ValueInt)
	{
	  alignValue = value->Data.Int.i;
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
      else if (Input_Match (',', false))
	{
	  const Value *valueO = exprBuildAndEval (ValueInt);
	  if (valueO->Tag == ValueInt)
	    offsetValue = ((uint32_t)valueO->Data.Int.i) % alignValue;
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

  uint32_t curPos = (areaCurrentSymbol->area.info->type & AREA_ABS) ? Area_GetBaseAddress (areaCurrentSymbol) : 0;
  curPos += areaCurrentSymbol->area.info->curIdx;
  uint32_t newPos = ((curPos - offsetValue + alignValue - 1) / alignValue)*alignValue + offsetValue;  
  uint32_t bytesToStuff = newPos - curPos;

  Area_EnsureExtraSize (bytesToStuff);

  while (bytesToStuff--)
    areaCurrentSymbol->area.info->image[areaCurrentSymbol->area.info->curIdx++] = 0;

  return false;
}


/**
 * \param align Needs to be power of 2 except value 0.
 * \param msg When non-NULL, a warning will be given prefixed with "Unaligned ".
 */
size_t
Area_AlignTo (size_t offset, int align, const char *msg)
{
  assert (align && (align & (align - 1)) == 0);
  if (msg && (offset & (align - 1)) != 0)
    error (ErrorWarning, "Unaligned %s", msg);
  size_t newOffset = (offset + align-1) & -align;
  Area_EnsureExtraSize (newOffset - areaCurrentSymbol->area.info->curIdx);
  if (areaCurrentSymbol->area.info->curIdx < newOffset)
    {
      for (size_t i = areaCurrentSymbol->area.info->curIdx; i != newOffset; ++i)
	areaCurrentSymbol->area.info->image[i] = 0;
      areaCurrentSymbol->area.info->curIdx = newOffset;
    }
  return newOffset;
}


/**
 * Implements '%' and 'SPACE'.
 */
bool
c_reserve (void)
{
  const Value *value = exprBuildAndEval (ValueInt);
  if (value->Tag == ValueInt)
    {
      if (value->Data.Int.i < 0)
	error (ErrorWarning, "Reserve space value is considered unsigned, i.e. reserving %u bytes now\n", value->Data.Int.i);
      Area_EnsureExtraSize ((unsigned)value->Data.Int.i);

      size_t i = areaCurrentSymbol->area.info->curIdx;
      areaCurrentSymbol->area.info->curIdx += value->Data.Int.i;
      memset (&areaCurrentSymbol->area.info->image[i], 0, (unsigned)value->Data.Int.i);
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
    {
      if (!(sym->type & SYMBOL_AREA))
	error (ErrorError, "Redefinition of label to area %s", sym->str);
    }
  else
    {
      sym->type = SYMBOL_AREA | SYMBOL_LOCAL;
      sym->value = Value_Int (0);
      sym->area.info = areaNew (sym, AREA_CODE | AREA_READONLY | AREA_DEFAULT_ALIGNMENT);
    }

  areaCurrentSymbol = sym;
  oArea_CurrentEntryType = eInvalid;
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
    return false; /* No need to give an error, lexGetId already did.  */

  Symbol *sym = symbolGet (&lex);
  int oldtype = 0;  
  if (sym->type & SYMBOL_DEFINED)
    {
      if (sym->type & SYMBOL_AREA)
	oldtype = sym->area.info->type;
      else
	error (ErrorError, "Redefinition of label as area %s", sym->str);
    }
  else
    {
      sym->type = SYMBOL_AREA | SYMBOL_LOCAL;
      sym->value = Value_Int (0);
      sym->area.info = areaNew (sym, 0);
    }
  skipblanks ();

  int newtype = 0;
  bool rel_specified = false, data_specified = false;
  while (Input_Match (',', true))
    {
      Lex attribute = lexGetId ();
      if (attribute.Data.Id.len == sizeof ("ABS")-1
	  && !memcmp ("ABS", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  if (rel_specified)
	    error (ErrorError, "Conflicting area attributes ABS vs REL");
	  newtype |= AREA_ABS;
	}
      else if (attribute.Data.Id.len == sizeof ("REL")-1
	       && !memcmp ("REL", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  if (newtype & AREA_ABS)
	    error (ErrorError, "Conflicting area attributes ABS vs REL");
	  rel_specified = true;
	}
      else if (attribute.Data.Id.len == sizeof ("CODE")-1
	       && !memcmp ("CODE", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  if (data_specified)
	    error (ErrorError, "Conflicting area attributes CODE vs DATA");
	  newtype |= AREA_CODE;
	}
      else if (attribute.Data.Id.len == sizeof ("DATA")-1
	       && !memcmp ("DATA", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  if (newtype & AREA_CODE)
	    error (ErrorError, "Conflicting area attributes CODE vs DATA");
	  data_specified = true;
	}
      else if (attribute.Data.Id.len == sizeof ("COMDEF")-1
	       && !memcmp ("COMDEF", attribute.Data.Id.str, attribute.Data.Id.len))
	newtype |= AREA_COMMONDEF;
      else if (attribute.Data.Id.len == sizeof ("COMMON")-1 /* == sizeof ("COMREF")-1 */
	       && (!memcmp ("COMMON", attribute.Data.Id.str, attribute.Data.Id.len)
		   || !memcmp ("COMREF", attribute.Data.Id.str, attribute.Data.Id.len)))
	newtype |= AREA_COMMONREF | AREA_UDATA;
      else if (attribute.Data.Id.len == sizeof ("NOINIT")-1
	       && !memcmp ("NOINIT", attribute.Data.Id.str, attribute.Data.Id.len))
	newtype |= AREA_UDATA;
      else if (attribute.Data.Id.len == sizeof ("READONLY")-1
	       && !memcmp ("READONLY", attribute.Data.Id.str, attribute.Data.Id.len))
	newtype |= AREA_READONLY;
      else if (attribute.Data.Id.len == sizeof ("PIC")-1
	       && !memcmp ("PIC", attribute.Data.Id.str, attribute.Data.Id.len))
	newtype |= AREA_PIC;
      else if (attribute.Data.Id.len == sizeof ("DEBUG")-1
	       && !memcmp ("DEBUG", attribute.Data.Id.str, attribute.Data.Id.len))
	newtype |= AREA_DEBUG;
      else if (attribute.Data.Id.len == sizeof ("REENTRANT")-1
	       && !memcmp ("REENTRANT", attribute.Data.Id.str, attribute.Data.Id.len))
	newtype |= AREA_REENTRANT;
      else if (attribute.Data.Id.len == sizeof ("BASED")-1
	       && !memcmp ("BASED", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  skipblanks ();
	  ARMWord reg = getCpuReg ();
	  newtype |= AREA_BASED | (reg << 24);
	}
      else if (attribute.Data.Id.len == sizeof ("LINKONCE")-1
	       && !memcmp ("LINKONCE", attribute.Data.Id.str, attribute.Data.Id.len))
	newtype |= AREA_LINKONCE;
      else if (attribute.Data.Id.len == sizeof ("ALIGN")-1
	       && !memcmp ("ALIGN", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  if (newtype & AREA_ALIGN_MASK)
	    error (ErrorError, "You can't specify ALIGN attribute more than once");
	  skipblanks ();
	  if (!Input_Match ('=', false))
	    error (ErrorError, "Malformed ALIGN attribute specification");
	  const Value *value = exprBuildAndEval (ValueInt);
	  if (value->Tag == ValueInt)
	    {
	      if (value->Data.Int.i < 2 || value->Data.Int.i > 31)
		error (ErrorError, "ALIGN attribute value must be between 2 (incl) and 31 (incl)");
	      else
		newtype |= value->Data.Int.i;
	    }
	  else
	    error (ErrorError, "Unrecognized ALIGN attribute value");
	}
      else
	error (ErrorError, "Illegal area attribute %.*s",
	       (int)attribute.Data.Id.len, attribute.Data.Id.str);
      skipblanks ();
    }

  /* Pending ORG to be taken into account ? */
  if (oPendingORG.isValid)
    {
      newtype |= AREA_ABS;
      sym->value.Data.Int.i = oPendingORG.value;
      oPendingORG.isValid = false;
    }

  /* Any alignment specified ? No, take default alignment (2) */
  if ((newtype & AREA_ALIGN_MASK) == 0)
    newtype |= AREA_DEFAULT_ALIGNMENT;

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
      if (gOptionAPCS & APCS_OPT_32BIT)
	newtype |= AREA_32BITAPCS;
      if (gOptionAPCS & APCS_OPT_FPREGARGS)
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

  /* When an area is made absolute, ensure its symbol is also absolute.  */
  if (newtype & AREA_ABS)
    sym->type |= SYMBOL_ABSOLUTE;
  
  /* We ignore any ABS difference as we like this to work:
	AREA Code, CODE
	ORG &xxx
	AREA Code, CODE  */
  if (newtype && oldtype
      && (newtype & ~AREA_ABS) != (oldtype & ~AREA_ABS))
    error (ErrorWarning, "Change in attribute of area %s will be ignored", sym->str);
  else
    sym->area.info->type |= newtype;

  areaCurrentSymbol = sym;
  oArea_CurrentEntryType = eInvalid;

  return false;
}

/**
 * Implements ORG.
 */
bool
c_org (void)
{
  const Value *value = exprBuildAndEval (ValueInt);
  if (value->Tag == ValueInt)
    {
      if (Area_IsImplicit (areaCurrentSymbol))
	{
	  if (oPendingORG.isValid)
	    {
	      errorLine (oPendingORG.file, oPendingORG.line, ErrorWarning, "ORG statement without any effect, because of...");
	      error (ErrorWarning, "...this");
	    }
	  else
	    oPendingORG.isValid = true;
	  oPendingORG.file = FS_GetCurFileName ();
	  oPendingORG.line = FS_GetCurLineNumber ();
	  oPendingORG.value = value->Data.Int.i;
	}
      else
	{
	  if (areaCurrentSymbol->area.info->curIdx != 0)
	    error (ErrorError, "Too late to set ORG of current area");
	  else
	    {
	      areaCurrentSymbol->area.info->type |= AREA_ABS;
	      areaCurrentSymbol->value.Data.Int.i = value->Data.Int.i;

	      /* When an area is made absolute, ensure its symbol is also
		 absolute.  */
	      areaCurrentSymbol->type |= SYMBOL_ABSOLUTE;
	    }
	}
    }
  else
    error (ErrorError, "ORG needs explicit address");

  return false;
}


/**
 * Implements PRESERVE8.
 */
bool
c_preserve8 (void)
{
  skipblanks ();
  if (Input_IsEolOrCommentStart ())
    gArea_Preserve8 = ePreserve8_Yes;
  else
    {
      const Value *value = exprBuildAndEval (ValueBool);
      if (value->Tag == ValueBool)
	gArea_Preserve8 = value->Data.Bool.b ? ePreserve8_Yes : ePreserve8_No;
      else
	error (ErrorError, "PRESERVE8 needs boolean argument");
    }
  return false;
}

/**
 * Implements REQUIRE8.
 */
bool
c_require8 (void)
{
  skipblanks ();
  if (Input_IsEolOrCommentStart ())
    gArea_Require8 = true;
  else
    {
      const Value *value = exprBuildAndEval (ValueBool);
      if (value->Tag == ValueBool)
	gArea_Require8 = value->Data.Bool.b;
      else
	error (ErrorError, "REQUIRE8 needs boolean argument");
    }
  return false;
}


/**
 * Mark (parts of) AREA containing data, ARM or Thumb instructions.
 * Used to implement mapping symbols.
 */
void
Area_MarkStartAs (Area_eEntryType type)
{
  assert (type != eInvalid);

  /* Don't bother doing this when we don't yet have an area or when it is
     the implicit one.  This will be faulted anyway later on.  */
  if (areaCurrentSymbol == NULL || Area_IsImplicit (areaCurrentSymbol))
    return;

  if (oArea_CurrentEntryType != type)
    {
      oArea_CurrentEntryType = type;

      const char *baseMappingSymbol;
      switch (type)
	{
	  case eARM:
	    baseMappingSymbol = "$a";
	    break;
	  case eData:
	    baseMappingSymbol = "$d";
	    break;
	  case eThumb:
	    baseMappingSymbol = "$t";
	    break;
	  case eInvalid:
	    break;
	}
      size_t mappingSymbolSize = 2 + 1 + areaCurrentSymbol->len + 1 + 8 + 1;
      char *mappingSymbol = alloca (mappingSymbolSize);
      int size = snprintf (mappingSymbol, mappingSymbolSize, "%s.%s.%08X",
			   baseMappingSymbol,
			   areaCurrentSymbol->str,
			   areaCurrentSymbol->area.info->curIdx);
      assert ((size_t)size + 1 == mappingSymbolSize);
      const Lex mapSymbolLex = lexTempLabel (mappingSymbol, mappingSymbolSize - 1);
      Symbol *label = ASM_DefineLabel (&mapSymbolLex, areaCurrentSymbol->area.info->curIdx);
      if (type == eData)
	label->type |= SYMBOL_DATUM;
    }
}

bool
Area_IsMappingSymbol (const char *symStr)
{
  return symStr[0] == '$'
	   && (symStr[1] == 'a' || symStr[1] == 'd' || symStr[1] == 't')
	   && (symStr[2] == '\0' || symStr[2] == '.');
}
