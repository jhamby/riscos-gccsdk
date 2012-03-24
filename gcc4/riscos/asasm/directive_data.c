/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 1997 Darren Salt
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
 * directive_data.c
 */

#include "config.h"

#include <assert.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#endif
#ifdef HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "area.h"
#include "code.h"
#include "directive_data.h"
#include "error.h"
#include "expr.h"
#include "fix.h"
#include "input.h"
#include "option.h"
#include "phase.h"
#include "put.h"
#include "state.h"


/**
 * The default align value size is depending on whether the area is DATA or
 * CODE.  In case of CODE, Thumb or ARM mode is deciding. 
 */
static uint32_t
GetDefaultAlignValueSize (void)
{
  if (areaCurrentSymbol->area.info->type & AREA_CODE)
    return State_GetInstrType () == eInstrType_ARM ? 4 : 2;
  return 1;
}


/**
 * Implements ALIGN [<power-of-2> [, <offset> [, <value> [, <value size>]]]]
 *
 * The current location is aligned to the next lowest address of the form:
 *   <offset> + n * <power-of-2>
 *   n is any integer which the assembler selects to minimise padding.
 */
bool
c_align (void)
{
  /* Determine alignment value.  */
  uint32_t alignValue;
  skipblanks ();
  if (Input_IsEolOrCommentStart ())
    alignValue = 1<<2;
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
    }

  /* Determine offset value.  */
  uint32_t offsetValue;
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

  /* Determine fill value.  */
  uint32_t fillValue;
  skipblanks ();
  if (Input_IsEolOrCommentStart ())
    fillValue = 0;
  else if (Input_Match (',', false))
    {
      const Value *valueF = exprBuildAndEval (ValueInt);
      if (valueF->Tag == ValueInt)
	fillValue = (uint32_t)valueF->Data.Int.i;
      else
	{
	  error (ErrorError, "Unrecognized ALIGN fill value");
	  fillValue = 0;
	}
    }
  else
    {
      error (ErrorError, "Unrecognized ALIGN fill value");
      fillValue = 0;
    }      

  /* Determine fill value size.  */
  uint32_t fillValueSize;
  skipblanks ();
  if (Input_IsEolOrCommentStart ())
    fillValueSize = GetDefaultAlignValueSize ();
  else if (Input_Match (',', false))
    {
      const Value *valueS = exprBuildAndEval (ValueInt);
      if (valueS->Tag == ValueInt)
	{
	  fillValueSize = valueS->Data.Int.i;
	  if (fillValueSize != 1 && fillValueSize != 2 && fillValueSize != 4)
	    {
	      error (ErrorError, "ALIGN value size needs to be 1, 2 or 4");
	      fillValueSize = GetDefaultAlignValueSize ();
	    }
	}
      else
	{
	  error (ErrorError, "Unrecognized ALIGN value size");
	  fillValueSize = GetDefaultAlignValueSize ();
	}
    }
  else
    {
      error (ErrorError, "Unrecognized ALIGN value size");
      fillValueSize = GetDefaultAlignValueSize ();
    }

  /* Check if given fill value requires more bits than specified (or implied)
     fill value size.  */
  if (Fix_CheckForOverflow (fillValueSize, fillValue))
    error (ErrorWarning, "Size value %" PRId32 " (= 0x%" PRIx32 ") exceeds %d byte%s",
           (int32_t)fillValue, (int32_t)fillValue, fillValueSize, fillValueSize == 1 ? "" : "s");

  /* We have to align on n x alignValue + offsetValue.  */

  uint32_t curPos = (areaCurrentSymbol->area.info->type & AREA_ABS) ? Area_GetBaseAddress (areaCurrentSymbol) : 0;
  curPos += areaCurrentSymbol->area.info->curIdx;
  uint32_t newPos = ((curPos - offsetValue + alignValue - 1) / alignValue)*alignValue + offsetValue;  
  uint32_t bytesToStuff = newPos - curPos;

  Area_EnsureExtraSize (areaCurrentSymbol, bytesToStuff);

  uint32_t curFillValue = fillValue >> 8*(curPos & (fillValueSize - 1));
  for (/* */; curPos != newPos; ++curPos)
    {
      if ((curPos & (fillValueSize - 1)) == 0)
	curFillValue = fillValue;
      areaCurrentSymbol->area.info->image[areaCurrentSymbol->area.info->curIdx++] = curFillValue;
      curFillValue >>= 8;
    }
  
  return false;
}

/**
 * Implements DATA (as nop).
 */
bool
c_data (void)
{
  return false;
}

/**
 * Reloc updater for DefineInt().
 */
bool
DefineInt_RelocUpdater (const char *fileName, unsigned lineNum, ARMWord offset,
			const Value *valueP, void *privData, bool final)
{
  const DefineInt_PrivData_t *privDataP = (const DefineInt_PrivData_t *)privData;

  assert (valueP->Tag == ValueCode && valueP->Data.Code.len != 0);

  /* Support for ValueString with data size 1 (i.e. DCB, =) and different
     data size but string length 1.  */
  if (valueP->Data.Code.len == 1
      && valueP->Data.Code.c[0].Tag == CodeValue
      && valueP->Data.Code.c[0].Data.value.Tag == ValueString
      && ((!final && privDataP->size == 1) || valueP->Data.Code.c[0].Data.value.Data.String.len == 1))
    {
      size_t len = valueP->Data.Code.c[0].Data.value.Data.String.len;
      const char *str = valueP->Data.Code.c[0].Data.value.Data.String.s;
      /* Lay out a string.  */
      for (size_t i = 0; i != len; ++i)
	Put_AlignDataWithOffset (offset + i, privDataP->size,
	                         (unsigned char)str[i], 1,
	                         !privDataP->allowUnaligned);
      return false;
    }

  if (!final)
    {
      Put_AlignDataWithOffset (offset, privDataP->size, 0, 1,
                               !privDataP->allowUnaligned);
      return true;
    }

  /* Figure out relocation type(s).
   * relocs : Number of symbol/area relocations.
   * relative : Number of relocs against current area.  When strict positive,
   *   they become normal relocs.  When negate, -relative relocs will be
   *   PC-relative ones (so -relative <= relocs), the rest of the relocs
   *   will be ABS ones.
   */
  int relocs = 0;
  int relative = 0;
    {
      int factor = 1;
      for (size_t i = 0; i != valueP->Data.Code.len; ++i)
	{
	  if (i + 1 != valueP->Data.Code.len
	      && valueP->Data.Code.c[i + 1].Tag == CodeOperator
	      && valueP->Data.Code.c[i + 1].Data.op == eOp_Sub)
	    factor = -1;
	  else
	    factor = 1;

	  const Code *codeP = &valueP->Data.Code.c[i];
	  if (codeP->Tag == CodeOperator)
	    {
	      if (codeP->Data.op != eOp_Add && codeP->Data.op != eOp_Sub)
		return true;
	      continue;
	    }
	  assert (codeP->Tag == CodeValue);
	  const Value *valP = &codeP->Data.value;

	  switch (valP->Tag)
	    {
	      case ValueInt:
		break;

	      case ValueSymbol:
		{
		  Value value = *valP;
		  if (Value_ResolveSymbol (&value))
		    return true;
		  assert (value.Tag == ValueSymbol);
		  if (value.Data.Symbol.symbol == areaCurrentSymbol)
		    {
		      assert ((value.Data.Symbol.symbol->type & SYMBOL_ABSOLUTE) == 0);
		      relative += factor * value.Data.Symbol.factor;
		    }
		  else
		    {
		      if (factor * value.Data.Symbol.factor < 0)
			return true;
		      relocs += factor * value.Data.Symbol.factor;
		    }
		  break;
		}

	      default:
		return true;
	    }
	}
    }
  assert (relocs >= 0);
  if (relative < 0 && -relative > relocs)
    return true; /* More PC-rel relocs needed than relocs to be done.  */

  const uint32_t relocOffset = (!privDataP->allowUnaligned) ? (offset + privDataP->size - 1) & -privDataP->size : offset;

  ARMWord armValue = 0;
    {
      int factor = 1;
      for (size_t i = 0; i != valueP->Data.Code.len; ++i)
	{
	  if (i + 1 != valueP->Data.Code.len
	      && valueP->Data.Code.c[i + 1].Tag == CodeOperator
	      && valueP->Data.Code.c[i + 1].Data.op == eOp_Sub)
	    factor = -1;
	  else
	    factor = 1;

	  const Code *codeP = &valueP->Data.Code.c[i];
	  if (codeP->Tag == CodeOperator)
	    {
	      if (codeP->Data.op != eOp_Add && codeP->Data.op != eOp_Sub)
		return true;
	      continue;
	    }
	  assert (codeP->Tag == CodeValue);
	  const Value *valP = &codeP->Data.value;

	  switch (valP->Tag)
	    {
	      case ValueInt:
		armValue += factor * valP->Data.Int.i;
		break;

	      case ValueSymbol:
		{
		  Value value = *valP;
		  if (Value_ResolveSymbol (&value))
		    return true;
		  assert (value.Tag == ValueSymbol);
		  armValue += factor * value.Data.Symbol.offset;
		  int how;
		  switch (privDataP->size)
		    {
		      case 1:
			how = HOW2_INIT | HOW2_BYTE;
			break;
		      case 2:
			how = HOW2_INIT | HOW2_HALF;
			break;
		      case 4:
			how = HOW2_INIT | HOW2_WORD;
			break;
		      default:
			assert (0);
			break;
		    }
		  int numRelocs;
		  if (value.Data.Symbol.symbol == areaCurrentSymbol)
		    {
		      if (relative > 0)
			{
			  numRelocs = relative;
			  relative = 0;
			}
		      else
			numRelocs = 0; /* Nothing to be done.  */
		    }
		  else
		    {
		      numRelocs = factor * value.Data.Symbol.factor;
		      relocs -= numRelocs;
		    }
		  while (numRelocs--)
		    {
		      int how2;
		      if (relative < 0)
			{
			  ++relative;
			  how2 = how | HOW2_RELATIVE;
			}
		      else
			how2 = how;
		      if (Reloc_Create (how2, relocOffset, &value) == NULL)
			return true;
		    }
		  break;
		}

	      default:
		return true;
	    }
	}
      assert (!relocs && !relative);
    }
  armValue = Fix_Int (fileName, lineNum, privDataP->size, armValue);
  if (privDataP->size == 4 && privDataP->swapHalfwords)
    armValue = (armValue >> 16) | (armValue << 16);
  Put_AlignDataWithOffset (offset, privDataP->size, armValue, 1,
                           !privDataP->allowUnaligned);
  
  return false;
}

static void
DefineInt (int size, bool allowUnaligned, bool swapHalfwords, const char *mnemonic)
{
  DefineInt_PrivData_t privData =
    {
      .size = size,
      .allowUnaligned = allowUnaligned,
      .swapHalfwords = swapHalfwords
    };
  do
    {
      exprBuild ();
      ValueTag allowedTypes;
      switch (size)
	{
	  case 1:
	    allowedTypes = ValueInt | ValueString | ValueSymbol | ValueCode;
	    break;
	  case 8:
	    /* DCQ/DCQU only supports literals.  */
	    allowedTypes = ValueInt | ValueInt64;
	    break;
	  default:
	    allowedTypes = ValueInt | ValueSymbol | ValueCode;
	    break;
	}
      /* FIXME: test on size being 8 is a hack (Reloc_QueueExprUpdate is
         not ready for it).  */
      if (gPhase == ePassOne || size == 8)
	{
	  const Value *result = codeEval (allowedTypes, NULL);
	  switch (result->Tag)
	    {
	      case ValueString:
		{
		  size_t len = result->Data.String.len;
		  const char *str = result->Data.String.s;
		  /* Lay out a string.  */
		  for (size_t i = 0; i != len; ++i)
		    Put_AlignDataWithOffset (areaCurrentSymbol->area.info->curIdx,
					     privData.size, (unsigned char)str[i],
					     1, !privData.allowUnaligned);
		  break;
		}

              case ValueInt:
		Put_AlignDataWithOffset (areaCurrentSymbol->area.info->curIdx,
					 privData.size, result->Data.Int.i, 1,
					 !privData.allowUnaligned);
		break;
		
	      case ValueInt64:
		{
		  assert (size == 8);
		  const uint64_t val = result->Data.Int64.i;
		  Put_AlignDataWithOffset (areaCurrentSymbol->area.info->curIdx,
					   4, (uint32_t)val, 1,
					   !privData.allowUnaligned);
		  Put_AlignDataWithOffset (areaCurrentSymbol->area.info->curIdx,
					   4, (uint32_t)(val >> 32), 1,
					   !privData.allowUnaligned);
		  break;
		}

	      case ValueIllegal:
		error (ErrorError, "Illegal %s expression", mnemonic);
		break;
		
	      case ValueSymbol:
	      case ValueCode:
		Put_AlignDataWithOffset (areaCurrentSymbol->area.info->curIdx,
					 privData.size, 0, 1,
					 !privData.allowUnaligned);
		break;

	      default:
		assert (0);
		break;
	    }
	}
      else if (Reloc_QueueExprUpdate (DefineInt_RelocUpdater, areaCurrentSymbol->area.info->curIdx,
				      allowedTypes,
				      &privData, sizeof (privData)))
	error (ErrorError, "Illegal %s expression", mnemonic);

      skipblanks ();
    }
  while (Input_Match (',', false));
}


/**
 * Implements DCB and = (8 bit integer).
 * "Define Constant Byte"
 */
bool
c_dcb (void)
{
  DefineInt (1, true, false, "DCB or =");
  return false;
}


/**
 * Implements DCW, DCWU (16 bit integer).
 * "Define Constant Word"
 */
bool
c_dcw (bool doLowerCase)
{
  bool allowUnaligned = Input_Match (doLowerCase ? 'u' : 'U', false);
  if (!Input_IsEndOfKeyword ())
    return true;
  DefineInt (2, allowUnaligned, false, allowUnaligned ? "DCWU" : "DCW");
  return false;
}


/**
 * Implements & (32 bit integer).
 * "Define Constant Double-word"
 */
bool
c_ampersand (void)
{
  DefineInt (4, false, false, "&");
  return false;
}


/**
 * Implements DCD and DCDU (32 bit integer).
 * "Define Constant Double-word"
 */
bool
c_dcd (bool doLowerCase)
{
  bool allowUnaligned = Input_Match (doLowerCase ? 'u' : 'U', false);
  if (!Input_IsEndOfKeyword ())
    return true;
  DefineInt (4, allowUnaligned, false, allowUnaligned ? "DCDU" : "DCD");
  return false;
}


/**
 * Implements DCQ and DCQU (64 bit integer).
 *   {label} DCQ{U} {-}literal{,{-}literal}...
 */
bool
c_dcq (bool doLowerCase)
{
  bool allowUnaligned = Input_Match (doLowerCase ? 'u' : 'U', false);
  if (!Input_IsEndOfKeyword ())
    return true;
  DefineInt (8, allowUnaligned, false, allowUnaligned ? "DCQU" : "DCQ");
  return false;
}


/**
 * Implements DCI : "Define Constant Instruction"
 *   {label} DCI{.W} expr{,expr}
 * In ARM code: align to 4 bytes and write 4-byte value(s).
 * In Thumb code: align to 2 bytes and write 2-byte value(s).
 */
bool
c_dci (bool doLowerCase)
{
  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;
  unsigned alignValue;
  int instructionSize;
  bool swapHalfwords;
  if (State_GetInstrType () == eInstrType_ARM)
    {
      alignValue = 4;
      instructionSize = 4;
      swapHalfwords = false;
    }
  else
    {
      alignValue = 2;
      /* In Thumb mode, with DCI we're writing 2 byte instructions by default
         unless .W is explictly mentioned.  */
      instructionSize = instrWidth == eInstrWidth_Enforce32bit ? 4 : 2;
      swapHalfwords = true;
    }
  Area_AlignArea (areaCurrentSymbol, alignValue, "instruction");
  DefineInt (instructionSize, true, swapHalfwords, "DCI");
  return false;
}


/**
 * Reloc updater for DefineReal().
 */
bool
DefineReal_RelocUpdater (const char *fileName, unsigned lineNum, ARMWord offset,
			 const Value *valueP, void *privData, bool final)
{
  const DefineReal_PrivData_t *privDataP = (const DefineReal_PrivData_t *)privData;

  assert (valueP->Tag == ValueCode && valueP->Data.Code.len != 0);

  for (size_t i = 0; i != valueP->Data.Code.len; ++i)
    {
      const Code *codeP = &valueP->Data.Code.c[i];
      if (codeP->Tag == CodeOperator)
	{
	  if (codeP->Data.op != eOp_Add)
	    return true;
	  continue;
	}
      assert (codeP->Tag == CodeValue);
      const Value *valP = &codeP->Data.value;

      switch (valP->Tag)
	{
	  case ValueInt:
	    Put_FloatDataWithOffset (offset, privDataP->size, valP->Data.Int.i,
				     !privDataP->allowUnaligned);
	    break;

	  case ValueFloat:
	    Put_FloatDataWithOffset (offset, privDataP->size, valP->Data.Float.f,
				     !privDataP->allowUnaligned);
	    break;

	  case ValueSymbol:
	    if (!final)
	      {
		Put_FloatDataWithOffset (offset, privDataP->size, 0.,
					 !privDataP->allowUnaligned);
		return true;
	      }
	    errorLine (fileName, lineNum, ErrorError, "Can't create relocation");
	    break;

	  default:
	    return true;
	}
    }
  
  return false;
}

static void
DefineReal (int size, bool allowUnaligned, const char *mnemonic)
{
  DefineReal_PrivData_t privData =
    {
      .size = size,
      .allowUnaligned = allowUnaligned
    };
  do
    {
      exprBuild ();
      if (gPhase == ePassOne)
	Put_FloatDataWithOffset (areaCurrentSymbol->area.info->curIdx,
				 privData.size, 0., !privData.allowUnaligned);
      else
	{
	  if (Reloc_QueueExprUpdate (DefineReal_RelocUpdater, areaCurrentSymbol->area.info->curIdx,
				     ValueFloat | ValueSymbol | ValueCode, &privData, sizeof (privData)))
	    error (ErrorError, "Illegal %s expression", mnemonic);
	}
      
      skipblanks ();
    }
  while (Input_Match (',', false));
}

/**
 * Implements DCFS / DCFSU (IEEE Single Precision).
 * "Define Constant Float-single precision"
 */
bool
c_dcfs (bool doLowerCase)
{
  bool allowUnaligned = Input_Match (doLowerCase ? 'u' : 'U', false);
  if (!Input_IsEndOfKeyword ())
    return true;
  DefineReal (4, allowUnaligned, allowUnaligned ? "DCFSU" : "DCFS");
  return false;
}

/**
 * Implements DCFD / DCFDU (IEEE Double Precision).
 * "Define Constant Float-double precision"
 */
bool
c_dcfd (bool doLowerCase)
{
  bool allowUnaligned = Input_Match (doLowerCase ? 'u' : 'U', false);
  if (!Input_IsEndOfKeyword ())
    return true;
  DefineReal (8, allowUnaligned, allowUnaligned ? "DCFDU" : "DCFD");
  return false;
}


/**
 * Used for '%', 'FILL' and 'SPACE' implementations.
 * \param isFill Is 'FILL' mnemonic.  Otherwise it is '%' or 'SPACE'.
 */
static void
ReserveSpace (bool isFill)
{
  const Value *valueTimesP = exprBuildAndEval (ValueInt);
  if (valueTimesP->Tag != ValueInt)
    {
      error (ErrorError, "Unresolved reserve not possible");
      return;
    }
  uint32_t times = (uint32_t)valueTimesP->Data.Int.i;

  ARMWord value = 0;
  unsigned valueSize = 1;
  if (isFill)
    {
      /* Check for <value> presence.  */
      if (Input_Match (',', false))
	{
	  const Value *valueValueP = exprBuildAndEval (ValueInt);
	  if (valueValueP->Tag != ValueInt)
	    {
	      error (ErrorError, "Unresolved reserve value not possible");
	      return;
	    }
	  value = (ARMWord)valueValueP->Data.Int.i;

	  /* Check for <valuesize> presence.  */
	  if (Input_Match (',', false))
	    {
	      const Value *valueSizeP = exprBuildAndEval (ValueInt);
	      if (valueSizeP->Tag != ValueInt)
		{
		  error (ErrorError, "Unresolved reserve value size not possible");
		  return;
		}
	      valueSize = (unsigned)valueSizeP->Data.Int.i;
	      if (valueSize != 1 && valueSize != 2 && valueSize != 4)
		{
		  error (ErrorError, "Reserve value size can only be 1, 2 or 4");
		  return;
		}
	    }
	}
    }
  
  if ((int32_t)times < 0)
    error (ErrorWarning, "Reserve space value is considered unsigned, i.e. reserving %u bytes now\n", times);

  Put_AlignDataWithOffset (areaCurrentSymbol->area.info->curIdx, valueSize,
			   value, times, false);
}


/**
 * Implements 'FILL'.
 *   {label} FILL <expr> {,<value>{,<valuesize>}}
 */
bool
c_fill (void)
{
  ReserveSpace (true);
  return false;
}


/**
 * Implements '%', 'SPACE'.
 *   {label} % <expr>
 *   {label} SPACE <expr>
 */
bool
c_reserve (void)
{
  ReserveSpace (false);
  return false;
}
