/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2012 GCCSDK Developers
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

#include <alloca.h>
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
#include "error.h"
#include "expr.h"
#include "filestack.h"
#include "get.h"
#include "input.h"
#include "lex.h"
#include "main.h"
#include "phase.h"
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
static const char *oArea_EntrySymbolFileName = NULL;
static unsigned oArea_EntrySymbolLineNum = 0;
int areaEntryOffset = 0;
Symbol *areaHeadSymbol = NULL;

/* FIXME: gArea_Require8 & gArea_Preserve8(Guessed) needs to be written in ELF output.  */
bool gArea_Require8 = false; /* Absense of REQUIRE8 => {FALSE} */
Preserve8_eValue gArea_Preserve8 = ePreserve8_Guess;
bool gArea_Preserve8Guessed = true;

static struct PendingORG
{
  bool isValid; /** true when we have a pending ORG statement.  */
  const char *fileName; /** Non-NULL for pending ORG statement and points to filename of that ORG.  */
  unsigned lineNum; /** Linenumber of pending ORG.  */
  uint32_t value; /** Pending ORG value.  */
} oPendingORG;

static void
Area_ResetPrivateVars (void)
{
  areaCurrentSymbol = NULL;
  oArea_CurrentEntryType = eInvalid;
  areaEntrySymbol = NULL;
  oArea_EntrySymbolFileName = NULL;
  oArea_EntrySymbolLineNum = 0;
  areaEntryOffset = 0;
  /* areaHeadSymbol = NULL; */

  gArea_Require8 = false; /* Absense of REQUIRE8 => {FALSE} */
  gArea_Preserve8 = ePreserve8_Guess;
  gArea_Preserve8Guessed = true;

  oPendingORG.isValid = false;
}

static Area *
areaNew (Symbol *sym, uint32_t type)
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
  res->relocs = NULL;

  res->litPool = NULL;

  res->number = 0;
  res->numRelocs = 0;
  
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
Area_EnsureExtraSize (Symbol *areaSym, size_t mingrow)
{
  if (areaSym->area.info->curIdx + mingrow <= areaSym->area.info->imagesize)
    return;

  assert (gPhase == ePassOne);
  
  /* When we want to grow an implicit area, it is time to give an error as
     this is not something we want to output.  */
  if (areaSym->area.info->imagesize == 0 && Area_IsImplicit (areaSym))
    error (ErrorError, "No area defined");

  size_t inc;
  if (areaSym->area.info->imagesize && areaSym->area.info->imagesize < DOUBLE_UP_TO)
    inc = areaSym->area.info->imagesize;
  else
    inc = GROWSIZE;
  if (inc < mingrow)
    inc = mingrow;
  while (inc > mingrow && !areaImage (areaSym->area.info, areaSym->area.info->imagesize + inc))
    inc /= 2;
  if (inc <= mingrow && !areaImage (areaSym->area.info, areaSym->area.info->imagesize + mingrow))
    errorAbort ("Area_EnsureExtraSize(): out of memory, minsize = %zd", mingrow);
}

/**
 * Area phase preparation.
 */
void
Area_PrepareForPhase (Phase_e phase)
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
	    errorLine (oPendingORG.fileName, oPendingORG.lineNum,
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
	    errorLine (oPendingORG.fileName, oPendingORG.lineNum,
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
  /* 'ENTRY' is not supported when in AAsm compatibility mode.  */
  if (option_abs)
    return true;

  if (Area_IsImplicit (areaCurrentSymbol))
    error (ErrorError, "No area selected before ENTRY");
  else
    {
      if (areaEntrySymbol)
	{
	  error (ErrorError, "More than one ENTRY");
	  errorLine (oArea_EntrySymbolFileName, oArea_EntrySymbolLineNum,
	             ErrorError, "note: Previous ENTRY was here"); 
	}
      else
	{
	  areaEntrySymbol = areaCurrentSymbol;
	  areaEntryOffset = areaCurrentSymbol->area.info->curIdx;
	  oArea_EntrySymbolFileName = FS_GetCurFileName ();
	  oArea_EntrySymbolLineNum = FS_GetCurLineNumber ();
	}
    }

  return false;
}


/**
 * Aligns given offset in given area for given align value.  If non-zero
 * alignment needs to be done, a warning is given based on given reason.
 * The current area index is updated when alignment surpasses it. 
 * \param areaSym Area symbol where alignment needs to be done.
 * \param offset Offset in area which needs to be aligned.
 * \param alignValue Alignment value, needs to be power of 2.
 * \param msg Reason for alignment When non-NULL, a warning will be given
 * prefixed with "Unaligned ".
 * \return Updated offset after alignment.
 */
uint32_t
Area_AlignOffset (Symbol *areaSym, uint32_t offset, unsigned alignValue, const char *msg)
{
  assert (areaSym->type & SYMBOL_AREA);
  assert (alignValue && (alignValue & (alignValue - 1)) == 0);
  if (msg && (offset & (alignValue - 1)) != 0)
    error (ErrorWarning, "Implicit aligning unaligned %s", msg);
  size_t newOffset = (offset + alignValue-1) & -alignValue;
  Area_EnsureExtraSize (areaSym, newOffset - areaSym->area.info->curIdx);
  if (areaSym->area.info->curIdx < newOffset)
    {
      memset (&areaSym->area.info->image[areaSym->area.info->curIdx], 0, newOffset - areaSym->area.info->curIdx); 
      areaSym->area.info->curIdx = newOffset;
    }
  return newOffset;
}


/**
 * Align given offset in the current area.
 * \see Area_AlignOffset.
 */
uint32_t
Area_AlignTo (uint32_t offset, unsigned alignValue, const char *msg)
{
  return Area_AlignOffset (areaCurrentSymbol, offset, alignValue, msg);
}


/**
 * Align the current index of given area.
 * \see Area_AlignOffset.
 */
uint32_t
Area_AlignArea (Symbol *areaSym, unsigned alignValue, const char *msg)
{
  return Area_AlignOffset (areaSym, areaSym->area.info->curIdx, alignValue, msg);
}


/**
 * Transforms AREA flags for code area according to the -APCS and -soft-float
 * option.
 */
static uint32_t
Area_ApplyAPCSOption (uint32_t areaFlags)
{
  /* 32-bit and FPv3 flags should only be set on CODE areas.  */
  if (gOptionAPCS & APCS_OPT_32BIT)
    areaFlags |= AREA_32BITAPCS;
  if (gOptionAPCS & APCS_OPT_FPREGARGS)
    areaFlags |= AREA_EXTFPSET;
  if (option_apcs_softfloat)
    areaFlags |= AREA_SOFTFLOAT;
  return areaFlags;
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
  const char *areaNameP;
  size_t areaNameSize;
  uint32_t areaType;
  if (option_abs)
    {
      /* AAsm compatibility.  */
      areaNameP = "ABS$$BLOCK";
      areaNameSize = sizeof ("ABS$$BLOCK")-1;
      areaType = Area_ApplyAPCSOption (AREA_CODE | AREA_DEFAULT_ALIGNMENT);
    }
  else
    {
      areaNameP = IMPLICIT_AREA_NAME;
      areaNameSize = sizeof (IMPLICIT_AREA_NAME)-1;
      areaType = AREA_CODE | AREA_ABS | AREA_READONLY | AREA_DEFAULT_ALIGNMENT;
    }
  const Lex lex = lexTempLabel (areaNameP, areaNameSize);
  Symbol *sym = symbolGet (&lex);
  if (SYMBOL_KIND (sym->type))
    error (ErrorError, "Redefinition of label to area %s", sym->str);
  else if ((sym->type & SYMBOL_AREA) == 0)
    {
      sym->type = SYMBOL_AREA;
      sym->value = Value_Int (0, eIntType_PureInt);
      sym->area.info = areaNew (sym, areaType);
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
  /* 'AREA' is not supported when in AAsm compatibility mode.  */
  if (option_abs)
    return true;

  Lex lex = lexGetId ();
  if (lex.tag != LexId)
    return false; /* No need to give an error, lexGetId already did.  */

  Symbol *sym = symbolGet (&lex);
  if (SYMBOL_KIND (sym->type))
    {
      error (ErrorError, "Redefinition of label as area %s", sym->str);
      Input_Rest ();
      return false;
    }
  unsigned int oldAreaType;  
  if (sym->type & SYMBOL_AREA)
    oldAreaType = sym->area.info->type;
  else
    {
      oldAreaType = 0;
      sym->type = SYMBOL_AREA;
      sym->value = Value_Int (0, eIntType_PureInt);
      sym->area.info = areaNew (sym, 0);
    }
  skipblanks ();

  unsigned int newAreaType = 0;
  bool rel_specified = false, data_specified = false;
  while (Input_Match (',', true))
    {
      Lex attribute = lexGetId ();
      if (attribute.Data.Id.len == sizeof ("ABS")-1
	  && !memcmp ("ABS", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  if (rel_specified)
	    error (ErrorError, "Conflicting area attributes ABS vs REL");
	  newAreaType |= AREA_ABS;
	}
      else if (attribute.Data.Id.len == sizeof ("REL")-1
	       && !memcmp ("REL", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  if (newAreaType & AREA_ABS)
	    error (ErrorError, "Conflicting area attributes ABS vs REL");
	  rel_specified = true;
	}
      else if (attribute.Data.Id.len == sizeof ("CODE")-1
	       && !memcmp ("CODE", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  if (data_specified)
	    error (ErrorError, "Conflicting area attributes CODE vs DATA");
	  newAreaType |= AREA_CODE;
	}
      else if (attribute.Data.Id.len == sizeof ("DATA")-1
	       && !memcmp ("DATA", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  if (newAreaType & AREA_CODE)
	    error (ErrorError, "Conflicting area attributes CODE vs DATA");
	  data_specified = true;
	}
      else if (attribute.Data.Id.len == sizeof ("COMDEF")-1
	       && !memcmp ("COMDEF", attribute.Data.Id.str, attribute.Data.Id.len))
	newAreaType |= AREA_COMMONDEF;
      else if (attribute.Data.Id.len == sizeof ("COMMON")-1 /* == sizeof ("COMREF")-1 */
	       && (!memcmp ("COMMON", attribute.Data.Id.str, attribute.Data.Id.len)
		   || !memcmp ("COMREF", attribute.Data.Id.str, attribute.Data.Id.len)))
	newAreaType |= AREA_COMMONREF | AREA_UDATA;
      else if (attribute.Data.Id.len == sizeof ("NOINIT")-1
	       && !memcmp ("NOINIT", attribute.Data.Id.str, attribute.Data.Id.len))
	newAreaType |= AREA_UDATA;
      else if (attribute.Data.Id.len == sizeof ("READONLY")-1
	       && !memcmp ("READONLY", attribute.Data.Id.str, attribute.Data.Id.len))
	newAreaType |= AREA_READONLY;
      else if (attribute.Data.Id.len == sizeof ("PIC")-1
	       && !memcmp ("PIC", attribute.Data.Id.str, attribute.Data.Id.len))
	newAreaType |= AREA_PIC;
      else if (attribute.Data.Id.len == sizeof ("DEBUG")-1
	       && !memcmp ("DEBUG", attribute.Data.Id.str, attribute.Data.Id.len))
	newAreaType |= AREA_DEBUG;
      else if (attribute.Data.Id.len == sizeof ("REENTRANT")-1
	       && !memcmp ("REENTRANT", attribute.Data.Id.str, attribute.Data.Id.len))
	newAreaType |= AREA_REENTRANT;
      else if (attribute.Data.Id.len == sizeof ("BASED")-1
	       && !memcmp ("BASED", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  skipblanks ();
	  ARMWord reg = getCpuReg ();
	  newAreaType |= AREA_BASED | (reg << 24);
	}
      else if (attribute.Data.Id.len == sizeof ("LINKONCE")-1
	       && !memcmp ("LINKONCE", attribute.Data.Id.str, attribute.Data.Id.len))
	newAreaType |= AREA_LINKONCE;
      else if (attribute.Data.Id.len == sizeof ("ALIGN")-1
	       && !memcmp ("ALIGN", attribute.Data.Id.str, attribute.Data.Id.len))
	{
	  if (newAreaType & AREA_ALIGN_MASK)
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
		newAreaType |= value->Data.Int.i;
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
      newAreaType |= AREA_ABS;
      sym->value.Data.Int.i = oPendingORG.value;
      oPendingORG.isValid = false;
    }

  /* Any alignment specified ? No, take default alignment (2) */
  if ((newAreaType & AREA_ALIGN_MASK) == 0)
    newAreaType |= AREA_DEFAULT_ALIGNMENT;

  /* AREA_COMMONDEF + AREA_COMMONREF => AREA_COMMONDEF */
  if (newAreaType & AREA_COMMONDEF)
    newAreaType &= ~AREA_COMMONREF;

  /* AREA_COMMONDEF + AREA_UDATA => AREA_COMMONREF */
  if ((newAreaType & AREA_COMMONDEF) && (newAreaType & AREA_UDATA))
    {
      newAreaType &= ~(AREA_COMMONDEF | AREA_UDATA);
      newAreaType |= AREA_COMMONREF;
    }

  /* Debug area ? Ignore code attribute */
  if (newAreaType & AREA_DEBUG)
    newAreaType &= ~AREA_CODE;

  if (newAreaType & AREA_CODE)
    {
      /* CODE area.  */
      newAreaType = Area_ApplyAPCSOption (newAreaType);

      if (newAreaType & AREA_BASED)
	{
	  newAreaType &= ~AREA_BASED;
	  error (ErrorError, "Attribute BASED may not be set for CODE area");
	}
    }
  else
    {
      /* DATA area.  */
      assert (!(newAreaType & (AREA_32BITAPCS | AREA_EXTFPSET | AREA_NOSTACKCHECK)));
      if (newAreaType & AREA_REENTRANT)
	{
	  newAreaType &= ~AREA_REENTRANT;
	  error (ErrorError, "Attribute REENTRANT may not be set for a DATA area");
	}
    }
  
  if ((newAreaType & AREA_READONLY) && (newAreaType & AREA_UDATA))
    error (ErrorError, "Attributes READONLY and NOINIT are mutually exclusive");

  if ((newAreaType & AREA_LINKONCE) && !(newAreaType & AREA_COMMONDEF))
    error (ErrorError, "Attribute LINKONCE must appear as part of a COMDEF");

  /* When an area is made absolute, ensure its symbol is also absolute.  */
  if (newAreaType & AREA_ABS)
    sym->type |= SYMBOL_ABSOLUTE;
  
  /* We ignore any ABS difference as we like this to work:
	AREA Code, CODE
	ORG &xxx
	AREA Code, CODE  */
  if (newAreaType && oldAreaType
      && (newAreaType & ~AREA_ABS) != (oldAreaType & ~AREA_ABS))
    error (ErrorWarning, "Change in attribute of area %s will be ignored", sym->str);
  else
    sym->area.info->type |= newAreaType;

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
	      errorLine (oPendingORG.fileName, oPendingORG.lineNum,
	                 ErrorWarning, "ORG statement without any effect, because of...");
	      error (ErrorWarning, "...this");
	    }
	  else
	    oPendingORG.isValid = true;
	  oPendingORG.fileName = FS_GetCurFileName ();
	  oPendingORG.lineNum = FS_GetCurLineNumber ();
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
Area_MarkStartAs (const Symbol *areaSymbol, uint32_t offset, Area_eEntryType type)
{
  assert (type != eInvalid);

  /* Don't bother doing this when we don't yet have an area or when it is
     the implicit one.  This will be faulted anyway later on.  */
  if (areaSymbol == NULL || Area_IsImplicit (areaSymbol))
    return;

  if (oArea_CurrentEntryType != type)
    {
      oArea_CurrentEntryType = type;

      char baseMappingSymbol;
      switch (type)
	{
	  case eARM:
	    baseMappingSymbol = 'a';
	    break;
	  case eData:
	    baseMappingSymbol = 'd';
	    break;
	  case eThumb:
	    baseMappingSymbol = 't';
	    break;
	  case eInvalid:
	    break;
	}
      size_t mappingSymbolSize = 2 + 1 + areaSymbol->len + 1 + 8 + 1;
      char *mappingSymbol = alloca (mappingSymbolSize);
      int size = snprintf (mappingSymbol, mappingSymbolSize, "$%c.%s.%08X",
			   baseMappingSymbol,
			   areaSymbol->str,
			   offset);
      assert ((size_t)size + 1 == mappingSymbolSize);
      const Lex mapSymbolLex = lexTempLabel (mappingSymbol, mappingSymbolSize - 1);
      Symbol *label = ASM_DefineLabel (&mapSymbolLex, offset);
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
