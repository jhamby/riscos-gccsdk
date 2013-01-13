/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2013 GCCSDK Developers
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
#include "lit.h"
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
Area_Create (Symbol *sym, uint32_t type)
{
  Area *newAreaP;
  if ((newAreaP = malloc (sizeof (Area))) == NULL)
    Error_OutOfMem ();

  newAreaP->next = areaHeadSymbol;
  newAreaP->type = type;
  newAreaP->imagesize = 0;
  newAreaP->image = NULL;

  newAreaP->curIdx = 0;
  newAreaP->maxIdx = 0;

  newAreaP->entryType = eInvalid;

  newAreaP->fileName = FS_GetCurFileName ();
  newAreaP->lineNumber = FS_GetCurLineNumber ();
  
  newAreaP->relocs = NULL;

  newAreaP->litPool = NULL;

  IT_InitializeState (&newAreaP->it);
  
  newAreaP->number = 0;
  newAreaP->numRelocs = 0;
  
  areaHeadSymbol = sym;

  return newAreaP;
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
 * Check if the given ORG value does not violate the area alignment value.
 */
static uint32_t
ValidateORGValue (uint32_t alignValue, uint32_t org)
{
  if (org & (alignValue - 1))
    Error (ErrorWarning, "Area ORG value 0x%x is not aligned according to area alignment value 0x%x",
           org, alignValue);
  return org;
}


/**
 * Ensures the current area has at least \t mingrow bytes free.
 */
void
Area_EnsureExtraSize (Symbol *areaSym, size_t mingrow)
{
  if (areaSym->area->curIdx + mingrow <= areaSym->area->imagesize)
    return;

  assert (gPhase == ePassOne);
  
  /* When we want to grow an implicit area, it is time to give an error as
     this is not something we want to output.  */
  if (areaSym->area->imagesize == 0 && Area_IsImplicit (areaSym))
    Error (ErrorError, "No area defined");

  size_t inc;
  if (areaSym->area->imagesize && areaSym->area->imagesize < DOUBLE_UP_TO)
    inc = areaSym->area->imagesize;
  else
    inc = GROWSIZE;
  if (inc < mingrow)
    inc = mingrow;
  while (inc > mingrow && !areaImage (areaSym->area, areaSym->area->imagesize + inc))
    inc /= 2;
  if (inc <= mingrow && !areaImage (areaSym->area, areaSym->area->imagesize + mingrow))
    Error_OutOfMem ();
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
	    Error_Line (oPendingORG.fileName, oPendingORG.lineNum,
		       ErrorWarning, "Unused ORG statement");

	  /* Do an implicit LTORG at the end of all areas.  */
	  for (areaCurrentSymbol = areaHeadSymbol;
	       areaCurrentSymbol != NULL;
	       areaCurrentSymbol = areaCurrentSymbol->area->next)
	    {
	      Lit_DumpPool ();
	      areaCurrentSymbol->area->maxIdx = areaCurrentSymbol->area->curIdx;
	      areaCurrentSymbol->area->curIdx = 0;
	    }

	  Area_ResetPrivateVars ();
	  Area_Ensure ();
	  break;
	}

      case eOutput:
	{
	  if (oPendingORG.isValid)
	    Error_Line (oPendingORG.fileName, oPendingORG.lineNum,
			ErrorWarning, "Unused ORG statement");

	  /* Do an implicit LTORG at the end of all areas.  */
	  for (areaCurrentSymbol = areaHeadSymbol;
	       areaCurrentSymbol != NULL;
	       areaCurrentSymbol = areaCurrentSymbol->area->next)
	    {
	      Lit_DumpPool ();
	      assert (areaCurrentSymbol->area->curIdx == areaCurrentSymbol->area->maxIdx);
	    }

	  /* Suggest NOINIT ? */
	  for (const Symbol *areaSymP = areaHeadSymbol; areaSymP != NULL; areaSymP = areaSymP->area->next)
	    {
	      const Area *areaP = areaSymP->area;
	      /* Skip suggestion when:
	          - READONLY area, as NOINIT can not be combined with READONLY.
	          - zero size area
	          - any relocs FIXME:
	          - non-zero data is in area.
	       */
	      if ((areaP->type & (AREA_READONLY | AREA_UDATA)) == 0
	          && areaP->relocs == NULL
	          && areaP->maxIdx != 0)
		{
		  uint32_t i;
		  for (i = 0; i != areaP->maxIdx && areaP->image[i] == 0; ++i)
		    /* */;
		  if (i == areaP->maxIdx)
		    Error_Line (areaP->fileName, areaP->lineNumber, ErrorInfo, "Area %s only contains zero bytes, use NOINIT area attribute ?", areaSymP->str);
		}
	    }
	  
	  /* Revert sort the area's so they become listed chronologically.  */
	  Symbol *aSymP = areaHeadSymbol;
	  assert (aSymP != NULL); /* There is always at least one area.  */
	  for (Symbol *nextSymP = aSymP->area->next; nextSymP != NULL; /* */)
	    {
	      Symbol *nextNextSymP = nextSymP->area->next;

	      nextSymP->area->next = aSymP;
	      aSymP = nextSymP;

	      nextSymP = nextNextSymP;
	    }
	  areaHeadSymbol->area->next = NULL;
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
    Error (ErrorError, "No area selected before ENTRY");
  else
    {
      if (areaEntrySymbol)
	{
	  Error (ErrorError, "More than one ENTRY");
	  Error_Line (oArea_EntrySymbolFileName, oArea_EntrySymbolLineNum,
	             ErrorError, "note: Previous ENTRY was here"); 
	}
      else
	{
	  areaEntrySymbol = areaCurrentSymbol;
	  areaEntryOffset = areaCurrentSymbol->area->curIdx;
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
    Error (ErrorWarning, "Implicit aligning unaligned %s", msg);
  size_t newOffset = (offset + alignValue-1) & -alignValue;
  Area_EnsureExtraSize (areaSym, newOffset - areaSym->area->curIdx);
  if (areaSym->area->curIdx < newOffset)
    {
      memset (&areaSym->area->image[areaSym->area->curIdx], 0, newOffset - areaSym->area->curIdx); 
      areaSym->area->curIdx = newOffset;
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
  return Area_AlignOffset (areaSym, areaSym->area->curIdx, alignValue, msg);
}


/**
 * Transforms AREA flags for code area according to the --APCS option.
 */
static uint32_t
Area_ApplyAPCSOption (uint32_t areaFlags)
{
  /* 32-bit and FPv3 flags should only be set on CODE areas.  */
  assert ((areaFlags & AREA_CODE) != 0);

  if ((gOptionAPCS & APCS_OPT_32BIT) != 0)
    areaFlags |= AREA_32BITAPCS;
  if ((gOptionAPCS & APCS_OPT_FPE3) != 0)
    areaFlags |= AREA_EXTFPSET;
  if ((gOptionAPCS & APCS_OPT_ROPI) != 0)
    areaFlags |= AREA_PIC;
  if ((gOptionAPCS & APCS_OPT_REENTRANT) != 0)
    areaFlags |= AREA_REENTRANT;
  if ((gOptionAPCS & APCS_OPT_SWSTACKCHECK) == 0)
    areaFlags |= AREA_NOSWSTACKCHECK;
  if ((gOptionAPCS & APCS_OPT_INTERWORK) != 0)
    areaFlags |= AREA_INTERWORK;

  if ((gOptionAPCS & APCS_OPT_VFPENDIAN) != 0)
    areaFlags |= AREA_VFP;

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
  const Lex lex = Lex_Id (areaNameP, areaNameSize);
  Symbol *sym = Symbol_Get (&lex);
  if (SYMBOL_KIND (sym->type) != 0)
    Error (ErrorError, "Redefinition of label to area %s", sym->str);
  else if ((sym->type & SYMBOL_AREA) == 0)
    {
      /* When an area is made absolute, ensure its symbol is also absolute.  */
      sym->type = (areaType & AREA_ABS) ? SYMBOL_ABSOLUTE | SYMBOL_AREA : SYMBOL_AREA;
      sym->value = Value_Int (0, eIntType_PureInt);
      sym->area = Area_Create (sym, areaType);
    }
  areaCurrentSymbol = sym;
}


bool
Area_IsImplicit (const Symbol *sym)
{
  return !strcmp (sym->str, IMPLICIT_AREA_NAME);
}

typedef enum
{
  eBothCodeAndData = 0,
  eForCodeAreaOnly = 1,
  eForDataAreaOnly = 2
} FlagCat_e;

typedef struct
{
  unsigned flagValues[3]; /**< '3' because FlagCat_e goes from 0 .. 2 (incl.).  */
  unsigned flagSets[3]; /**< '3' because FlagCat_e goes from 0 .. 2 (incl.).  */
} AttribResult_t;

/**
 * Parses the AREA BASED attribute.
 */
static bool
ParseAttributeBASED(AttribResult_t *result)
{
  Input_SkipWS ();
  unsigned reg = Get_CPUReg ();
  if (reg == INVALID_REG)
    return true;

  result->flagValues[eForDataAreaOnly] |= AREA_BASED | (reg << 24);
  result->flagSets[eForDataAreaOnly] |= AREA_BASED | (0xF << 24);
  return false;
}

/**
 * Parses the AREA ALIGN attribute.
 */
static bool
ParseAttributeALIGN(AttribResult_t *result)
{
  Input_SkipWS ();
  if (!Input_Match ('=', false))
    {
      Error (ErrorError, "Malformed ALIGN attribute specification");
      return true;
    }
  const Value *value = Expr_BuildAndEval (ValueInt);
  if (value->Tag != ValueInt || value->Data.Int.type != eIntType_PureInt)
    {
      Error (ErrorError, "Unrecognized ALIGN attribute value");
      return true;
    }
  if (value->Data.Int.i < 2 || value->Data.Int.i > 31)
    {
      Error (ErrorError, "ALIGN attribute value must be between 2 (incl) and 31 (incl)");
      return true;
    }

  result->flagValues[eBothCodeAndData] |= value->Data.Int.i;
  result->flagSets[eBothCodeAndData] |= AREA_ALIGN_MASK;
  return false;
}

static const struct AKeyword
{
  const char *keyword;
  size_t size;
  FlagCat_e category;
  unsigned flagValue;
  unsigned flagSet;
  bool (*parser)(AttribResult_t *result);
} oAttributes[] =
{
  { "CODE", sizeof ("CODE")-1, eBothCodeAndData, AREA_CODE, AREA_CODE, NULL }, 
  { "DATA", sizeof ("DATA")-1, eBothCodeAndData, 0, AREA_CODE, NULL },

  /* Attributes for both CODE and DATA areas.  */
  { "ABS", sizeof ("ABS")-1, eBothCodeAndData, AREA_ABS, AREA_ABS, NULL },
  { "REL", sizeof ("REL")-1, eBothCodeAndData, 0, AREA_ABS, NULL },
  { "PIC", sizeof ("PIC")-1, eBothCodeAndData, AREA_PIC, AREA_PIC, NULL },
  { "READONLY", sizeof ("READONLY")-1, eBothCodeAndData, AREA_READONLY, AREA_READONLY, NULL },
  { "READWRITE", sizeof ("READWRITE")-1, eBothCodeAndData, 0, AREA_READONLY, NULL },
  { "COMDEF", sizeof ("COMDEF")-1, eBothCodeAndData, AREA_COMMONDEF, AREA_COMMONDEF, NULL },
  { "COMMON", sizeof ("COMMON")-1, eBothCodeAndData, AREA_COMMONREF, AREA_COMMONREF, NULL },
  { "NOINIT", sizeof ("NOINIT")-1, eBothCodeAndData, AREA_UDATA, AREA_UDATA, NULL },
  { "ALIGN", sizeof ("ALIGN")-1, eBothCodeAndData, 0, AREA_ALIGN_MASK, ParseAttributeALIGN },
  { "VFP", sizeof ("VFP")-1, eBothCodeAndData, AREA_VFP, AREA_VFP, NULL },

  /* Attributes for DATA areas only.  */
  { "DEBUG", sizeof ("DEBUG")-1, eForDataAreaOnly, AREA_DEBUG, AREA_DEBUG, NULL },
  { "BASED", sizeof ("BASED")-1, eForDataAreaOnly, AREA_BASED, AREA_BASED | (0xF<<24), ParseAttributeBASED },

  /* Attributes for CODE areas only.  */
  { "REENTRANT", sizeof ("REENTRANT")-1, eForCodeAreaOnly, AREA_REENTRANT, AREA_REENTRANT, NULL },
  { "INTERWORK", sizeof ("INTERWORK")-1, eForCodeAreaOnly, AREA_INTERWORK, AREA_INTERWORK, NULL },
  { "HALFWORD", sizeof ("HALFWORD")-1, eForCodeAreaOnly, AREA_HALFWORD, AREA_HALFWORD, NULL },
  { "NOSWSTACKCHECK", sizeof ("NOSWSTACKCHECK")-1, eForCodeAreaOnly, AREA_NOSWSTACKCHECK, AREA_NOSWSTACKCHECK, NULL },
  { "CODEALIGN", sizeof ("CODEALIGN")-1, eForCodeAreaOnly, AREA_INT_CODEALIGN, AREA_INT_CODEALIGN, NULL }
};

/**
 * \return allocated in heap a string describing all the given area attribute
 * bits.
 */
static const char *
AttributesAsName (unsigned attributes)
{
  if (!attributes)
    return NULL;
  size_t len = 0;
  unsigned attributesCp = attributes;
  for (unsigned i = 0; i != sizeof (attributes)*8; ++i)
    {
      unsigned attribute = 1U << i;
      if (attributesCp & attribute)
	{
	  if (len)
	    len += sizeof (", ");
	  unsigned j;
	  for (j = 0; j != sizeof (oAttributes)/sizeof (oAttributes[0]); ++j)
	    {
	      if (attributesCp & attribute & oAttributes[j].flagSet)
		{
		  len += strlen (oAttributes[j].keyword);
		  attributesCp &= ~oAttributes[j].flagSet; /* To deal with multiple bits set in flagSet for one area attribute (like BASED).  */
		  break;
		}
	    }
	  if (j == sizeof (oAttributes)/sizeof (oAttributes[0]))
	    len += snprintf (NULL, 0, "attribute bit %d", i);
	}
    }
  char *result = malloc (len + 1);
  if (result == NULL)
    Error_OutOfMem ();
  result[0] = '\0';
  bool first = true;
  for (unsigned i = 0; i != sizeof (attributes)*8; ++i)
    {
      unsigned attribute = 1U << i;
      if (attributes & attribute)
	{
	  if (!first)
	    strcat (result, ", ");
	  else
	    first = false;
	  unsigned j;
	  for (j = 0; j != sizeof (oAttributes)/sizeof (oAttributes[0]); ++j)
	    {
	      if (attributes & attribute & oAttributes[j].flagSet)
		{
		  strcat (result, oAttributes[j].keyword); 
		  attributes &= ~oAttributes[j].flagSet; /* To deal with multiple bits set in flagSet for one area attribute (like BASED).  */
		  break;
		}
	    }
	  if (j == sizeof (oAttributes)/sizeof (oAttributes[0]))
	    sprintf (result + strlen (result), "attribute bit %d", i);
	}
    }
  return result;
}

static AttribResult_t
GetAreaAttributes (void)
{
  AttribResult_t result;
  memset (&result, 0, sizeof (result));
  while (Input_Match (',', true))
    {
      Lex attribute = Lex_GetID ();
      const struct AKeyword *keyword = NULL;
      for (size_t i = 0; i != sizeof (oAttributes)/sizeof (oAttributes[0]); ++i)
	{
	  if (attribute.Data.Id.len == oAttributes[i].size
	      && !memcmp (attribute.Data.Id.str, oAttributes[i].keyword, oAttributes[i].size))
	    {
	      keyword = &oAttributes[i];
	      break;
	    }
	}
      if (keyword == NULL)
	Error (ErrorError, "AREA attribute %.*s is not known",
	       (int)attribute.Data.Id.len, attribute.Data.Id.str);
      else
	{
	  if (keyword->parser)
	    {
	      if (keyword->parser (&result))
		break;
	    }
	  else if ((keyword->flagSet & result.flagSets[keyword->category]) != 0)
	    {
	      /* Attribute has already been set.  Verify consistency.  */
	      if ((keyword->flagSet & result.flagValues[keyword->category]) != keyword->flagValue)
		{
		  Error (ErrorError, "AREA attribute %.*s conflicts with a previously given attribute",
			 (int)attribute.Data.Id.len, attribute.Data.Id.str);
		  break;
		}
	    }
	  else
	    {
	      result.flagValues[keyword->category] |= keyword->flagValue;
	      result.flagSets[keyword->category] |= keyword->flagSet;
	    }
	}

      Input_SkipWS ();
    }

  return result;
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

  Lex lex = Lex_GetID ();
  if (lex.tag != LexId)
    return false; /* No need to give an error, Lex_GetID already did.  */

  Symbol *sym = Symbol_Get (&lex);
  if (SYMBOL_KIND (sym->type) != 0)
    {
      Error (ErrorError, "Redefinition of label as area %s", sym->str);
      return false;
    }
  unsigned int prevAreaAttrib;  
  if (sym->type & SYMBOL_AREA)
    {
      prevAreaAttrib = sym->area->type;
      assert (prevAreaAttrib);
    }
  else
    {
      prevAreaAttrib = 0;
      sym->type = SYMBOL_AREA;
      sym->value = Value_Int (0, eIntType_PureInt);
      sym->area = Area_Create (sym, 0);
    }
  Input_SkipWS ();

  AttribResult_t result = GetAreaAttributes ();

  /* Check if there are code area attributes set for a data area, or if there
     are data area attributes set for a code area.  */
  const char *areaType;
  unsigned wrongAreaAttributes;
  if ((result.flagValues[eBothCodeAndData] & AREA_CODE) != 0)
    {
      areaType = "CODE";
      result.flagValues[eBothCodeAndData] |= result.flagValues[eForCodeAreaOnly];
      result.flagSets[eBothCodeAndData] |= result.flagSets[eForCodeAreaOnly];
      wrongAreaAttributes = result.flagSets[eForDataAreaOnly];
    }
  else
    {
      areaType = "DATA";
      result.flagValues[eBothCodeAndData] |= result.flagValues[eForDataAreaOnly];
      result.flagSets[eBothCodeAndData] |= result.flagSets[eForDataAreaOnly];
      wrongAreaAttributes = result.flagSets[eForCodeAreaOnly];
    }
  if (wrongAreaAttributes)
    {
      const char *attrAsName = AttributesAsName (wrongAreaAttributes);
      Error (ErrorError, "A %s AREA can not have %s set", areaType, attrAsName);
      free ((void *)attrAsName);
    }

  /* When there is no alignment specified, take default alignment for new area
     definitions, or inherit from its previous definition.  */
  if ((result.flagValues[eBothCodeAndData] & AREA_ALIGN_MASK) == 0)
    result.flagValues[eBothCodeAndData] |= (prevAreaAttrib) ? (prevAreaAttrib & AREA_ALIGN_MASK) : AREA_DEFAULT_ALIGNMENT;

  /* Pending ORG to be taken into account ? */
  if (oPendingORG.isValid)
    {
      result.flagValues[eBothCodeAndData] |= AREA_ABS;
      uint32_t alignValue = 1U << (result.flagValues[eBothCodeAndData] & AREA_ALIGN_MASK);
      sym->value = Value_Int (ValidateORGValue (alignValue, oPendingORG.value), eIntType_PureInt);
      oPendingORG.isValid = false;
    }

  /* AREA_COMMONDEF + AREA_COMMONREF => AREA_COMMONDEF */
  if ((result.flagValues[eBothCodeAndData] & AREA_COMMONDEF) != 0)
    result.flagValues[eBothCodeAndData] &= ~AREA_COMMONREF;

  /* AREA_COMMONDEF + AREA_UDATA => AREA_COMMONREF */
  if ((result.flagValues[eBothCodeAndData] & (AREA_COMMONDEF | AREA_UDATA)) == (AREA_COMMONDEF | AREA_UDATA))
    {
      result.flagValues[eBothCodeAndData] &= ~(AREA_COMMONDEF | AREA_UDATA);
      result.flagValues[eBothCodeAndData] |= AREA_COMMONREF;
    }

  /* AREA_READONLY and AREA_UDATA can not be combined.  */
  if ((result.flagValues[eBothCodeAndData] & AREA_READONLY) != 0
      && (result.flagValues[eBothCodeAndData] & AREA_UDATA) != 0)
    Error (ErrorError, "Attributes READONLY and NOINIT are mutually exclusive");

  /* Apply APCS options on code area.  */
  if ((result.flagValues[eBothCodeAndData] & AREA_CODE) != 0)
    result.flagValues[eBothCodeAndData] = Area_ApplyAPCSOption (result.flagValues[eBothCodeAndData]);

  /* When an area is made absolute, ensure its symbol is also absolute.  */
  if ((result.flagValues[eBothCodeAndData] & AREA_ABS) != 0)
    sym->type |= SYMBOL_ABSOLUTE;

  /* When an area is re-defined, make sure its attributes are the same.  */
  if (prevAreaAttrib != 0)
    {
      unsigned changedAttr = result.flagValues[eBothCodeAndData] ^ prevAreaAttrib;
      /* We ignore any ABS difference as we like this to work without warning.
	   AREA Code, CODE
	   ORG &xxx
	   AREA Code, CODE
         Also ignore any area attribute bits which can only be set via
         Area_ApplyAPCSOption(), i.e. AREA_32BITAPCS and AREA_EXTFPSET.  */
      changedAttr &= ~(AREA_ABS | AREA_32BITAPCS | AREA_EXTFPSET);
      if (changedAttr)
	{
	  const char *attrAsName = AttributesAsName (changedAttr);
	  Error (ErrorWarning, "Change in attribute(s) %s for area %s will be ignored", attrAsName, sym->str);
	  free ((void *)attrAsName);
	}
    }
  else
    {
      /* Only set the area attribute bits when the area gets defined for the
         first time.  */
      sym->area->type |= result.flagValues[eBothCodeAndData];
      assert ((sym->area->type & ~((sym->area->type & AREA_CODE) ? AREA_INT_CODEMASK : AREA_INT_DATAMASK)) == 0);
    }

  areaCurrentSymbol = sym;

  return false;
}

/**
 * Implements ORG.
 */
bool
c_org (void)
{
  const Value *value = Expr_BuildAndEval (ValueInt);
  if (value->Tag == ValueInt && value->Data.Int.type == eIntType_PureInt)
    {
      if (Area_IsImplicit (areaCurrentSymbol))
	{
	  if (oPendingORG.isValid)
	    {
	      Error_Line (oPendingORG.fileName, oPendingORG.lineNum,
	                 ErrorWarning, "ORG statement without any effect, because of...");
	      Error (ErrorWarning, "...this");
	    }
	  else
	    oPendingORG.isValid = true;
	  oPendingORG.fileName = FS_GetCurFileName ();
	  oPendingORG.lineNum = FS_GetCurLineNumber ();
	  oPendingORG.value = value->Data.Int.i;
	}
      else
	{
	  if (areaCurrentSymbol->area->curIdx != 0)
	    Error (ErrorError, "Too late to set ORG of current area");
	  else
	    {
	      areaCurrentSymbol->area->type |= AREA_ABS;
	      areaCurrentSymbol->value = Value_Int (ValidateORGValue (1U << (areaCurrentSymbol->area->type & AREA_ALIGN_MASK), value->Data.Int.i), eIntType_PureInt);

	      /* When an area is made absolute, ensure its symbol is also
		 absolute.  */
	      areaCurrentSymbol->type |= SYMBOL_ABSOLUTE;
	    }
	}
    }
  else
    Error (ErrorError, "ORG needs explicit address");

  return false;
}


/**
 * Implements PRESERVE8.
 */
bool
c_preserve8 (void)
{
  Input_SkipWS ();
  if (Input_IsEolOrCommentStart ())
    gArea_Preserve8 = ePreserve8_Yes;
  else
    {
      const Value *value = Expr_BuildAndEval (ValueBool);
      if (value->Tag == ValueBool)
	gArea_Preserve8 = value->Data.Bool.b ? ePreserve8_Yes : ePreserve8_No;
      else
	Error (ErrorError, "PRESERVE8 needs boolean argument");
    }
  return false;
}

/**
 * Implements REQUIRE8.
 */
bool
c_require8 (void)
{
  Input_SkipWS ();
  if (Input_IsEolOrCommentStart ())
    gArea_Require8 = true;
  else
    {
      const Value *value = Expr_BuildAndEval (ValueBool);
      if (value->Tag == ValueBool)
	gArea_Require8 = value->Data.Bool.b;
      else
	Error (ErrorError, "REQUIRE8 needs boolean argument");
    }
  return false;
}


/**
 * Mark (parts of) AREA containing data, ARM, Thumb or ThumbEE instructions.
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

  if (areaSymbol->area->entryType != type)
    {
      areaSymbol->area->entryType = type;

      const char *baseMappingSymbol;
      switch (type)
	{
	  case eARM:
	    baseMappingSymbol = "a$$";
	    break;
	  case eData:
	    baseMappingSymbol = "d$$";
	    break;
	  case eThumb:
	    baseMappingSymbol = "t$$";
	    break;
	  case eThumbEE:
	    baseMappingSymbol = "t.x";
	    break;
	  case eInvalid:
	    break;
	}
      size_t mappingSymbolSize = 1 + 3 + 1 + areaSymbol->len + 1 + 8 + 1;
      char *mappingSymbol = alloca (mappingSymbolSize);
      int size = snprintf (mappingSymbol, mappingSymbolSize, "$%s.%s.%08X",
			   baseMappingSymbol,
			   areaSymbol->str,
			   offset);
      assert ((size_t)size + 1 == mappingSymbolSize);
      const Lex mapSymbolLex = Lex_Id (mappingSymbol, mappingSymbolSize - 1);
      (void) ASM_DefineLabel (&mapSymbolLex, offset, true);
    }
}


Area_eEntryType
Area_GetCurrentEntryType (void)
{
  return areaCurrentSymbol->area->entryType;
}


/**
 * Checks if given symbol is a mapping symbol (i.e. beginning with "$a",
 * "$d", "$t" or "$t.x").
 */
Area_eEntryType
Area_IsMappingSymbol (const char *symStr)
{
  if (symStr[0] == '$')
    {
      switch (symStr[1])
	{
	  case 'a':
	    return eARM;
	  case 'd':
	    return eData;
	  case 't':
	    return symStr[2] == '.' ? eThumbEE : eThumb;
	}
    }

  return eInvalid;
}
