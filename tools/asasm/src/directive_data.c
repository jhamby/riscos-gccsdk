/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 1997 Darren Salt
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
#include "it.h"
#include "option.h"
#include "phase.h"
#include "put.h"
#include "state.h"
#include "targetcpu.h"


/**
 * The default align value size is depending on whether the area is DATA or
 * CODE.  In case of CODE, Thumb or ARM mode is deciding. 
 */
static uint32_t
GetDefaultAlignValueSize (void)
{
  if (areaCurrentSymbol->area->type & AREA_CODE)
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
  Input_SkipWS ();
  bool alignValueProvided = false;
  uint32_t alignValue;
  if (Input_IsEolOrCommentStart ())
    {
    }
  else
    {
      /* Determine align value */
      const Value *valueA = Expr_BuildAndEval (ValueInt);
      if (valueA->Tag == ValueInt && valueA->Data.Int.type == eIntType_PureInt)
	{
	  alignValue = valueA->Data.Int.i;
	  if (alignValue != 0 && (alignValue & (alignValue - 1)) == 0)
	    alignValueProvided = true;
	  else
	    Error (ErrorError, "ALIGN value is not a power of two");
	}
      else
	Error (ErrorError, "Unrecognized ALIGN value");
    }
  if (!alignValueProvided)
    alignValue = 1<<2;

  /* Determine offset value.  */
  Input_SkipWS ();
  bool offsetValueProvided = false;
  uint32_t offsetValue;
  if (Input_IsEolOrCommentStart ())
    {
    }
  else if (Input_Match (',', false))
    {
      const Value *valueO = Expr_BuildAndEval (ValueInt);
      if (valueO->Tag == ValueInt && valueO->Data.Int.type == eIntType_PureInt)
	{
	  offsetValue = ((uint32_t)valueO->Data.Int.i) % alignValue;
	  offsetValueProvided = true;
	}
      else
	Error (ErrorError, "Unrecognized ALIGN offset value");
    }
  else
    Error (ErrorError, "Unrecognized ALIGN offset value");
  if (!offsetValueProvided)
    offsetValue = 0;

  /* Determine fill value.  */
  Input_SkipWS ();
  bool fillValueProvided = false;
  uint32_t fillValue;
  if (Input_IsEolOrCommentStart ())
    {
    }
  else if (Input_Match (',', false))
    {
      const Value *valueF = Expr_BuildAndEval (ValueInt);
      if (valueF->Tag == ValueInt && valueF->Data.Int.type == eIntType_PureInt)
	{
	  fillValue = (uint32_t)valueF->Data.Int.i;
	  fillValueProvided = true;
	}
      else
	Error (ErrorError, "Unrecognized ALIGN fill value");
    }
  else
    Error (ErrorError, "Unrecognized ALIGN fill value");
  if (!fillValueProvided)
    fillValue = 0;

  /* Determine fill value size.  */
  Input_SkipWS ();
  bool fillValueSizeProvided = false;
  uint32_t fillValueSize;
  if (Input_IsEolOrCommentStart ())
    {
    }
  else if (Input_Match (',', false))
    {
      const Value *valueS = Expr_BuildAndEval (ValueInt);
      if (valueS->Tag == ValueInt && valueS->Data.Int.type == eIntType_PureInt)
	{
	  fillValueSize = valueS->Data.Int.i;
	  if (fillValueSize == 1 || fillValueSize == 2 || fillValueSize == 4)
	    fillValueSizeProvided = true;
	  else
	    Error (ErrorError, "ALIGN value size needs to be 1, 2 or 4");
	}
      else
	Error (ErrorError, "Unrecognized ALIGN value size");
    }
  else
    Error (ErrorError, "Unrecognized ALIGN value size");
  if (!fillValueSizeProvided)
    fillValueSize = GetDefaultAlignValueSize ();

  /* Check if given fill value requires more bits than specified (or implied)
     fill value size.  */
  if (Fix_CheckForOverflow (fillValueSize, fillValue))
    Error (ErrorWarning, "Size value %" PRId32 " (= 0x%" PRIx32 ") exceeds %d byte%s",
           (int32_t)fillValue, (int32_t)fillValue, fillValueSize, fillValueSize == 1 ? "" : "s");

  /* We have to align on n x alignValue + offsetValue.  */

  uint32_t curPos = (areaCurrentSymbol->area->type & AREA_ABS) ? Area_GetBaseAddress (areaCurrentSymbol) : 0;
  curPos += areaCurrentSymbol->area->curIdx;
  uint32_t newPos = ((curPos - offsetValue + alignValue - 1) / alignValue)*alignValue + offsetValue;  
  uint32_t bytesToStuff = newPos - curPos;

  Area_EnsureExtraSize (areaCurrentSymbol, bytesToStuff);

  /* When area has CODEALIGN attribute, and <value> + <value size> are not
     specified, and ALIGN is following an instruction, and offset is multiple
     of current instruction size, then pad with NOP instructions.  */
  if ((areaCurrentSymbol->area->type & AREA_INT_CODEALIGN) != 0
      && !fillValueProvided
      && (offsetValue % fillValueSize) == 0
      && Area_GetCurrentEntryType () != eData) 
    {
      assert (((curPos - newPos) % fillValueSize) == 0);
      if (State_GetInstrType () == eInstrType_ARM)
	{
	  uint32_t instr = (Target_CheckCPUFeature (kCPUExt_v6K, false)
			    || Target_CheckCPUFeature (kCPUExt_v6T2, false)) ? 0xe320f000 : 0xE1A00000;
	  for (/* */; curPos != newPos; curPos += 4)
	    Put_Ins (4, instr);
	}
      else
	{
	  uint32_t instr = (Target_CheckCPUFeature (kCPUExt_v6K, false)
			    || Target_CheckCPUFeature (kCPUExt_v6T2, false)) ? 0xBF00 : 0x46c0;
	  for (/* */; curPos != newPos; curPos += 2)
	    Put_Ins (2, instr);
	}
    }
  else
    {
      uint32_t curFillValue = fillValue >> 8*(curPos & (fillValueSize - 1));
      for (/* */; curPos != newPos; ++curPos)
	{
	  if ((curPos & (fillValueSize - 1)) == 0)
	    curFillValue = fillValue;
	  areaCurrentSymbol->area->image[areaCurrentSymbol->area->curIdx++] = curFillValue;
	  curFillValue >>= 8;
	}
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
 * \return true when error occured.
 */
bool
DefineInt_HandleSymbols (int size, bool allowUnaligned, bool swapHalfwords,
			 uint32_t offset, const Value *valueP)
{
  assert (valueP->Tag == ValueCode);

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

	  Value value = *valP;
	  if (Value_ResolveSymbol (&value))
	    return true;
	  switch (value.Tag)
	    {
	      case ValueInt:
		break;

	      case ValueSymbol:
		{
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

  const uint32_t relocOffset = (!allowUnaligned) ? (offset + size - 1) & -size : offset;

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

	  Value value = *valP;
	  if (Value_ResolveSymbol (&value))
	    return true;
	  switch (value.Tag)
	    {
	      case ValueInt:
		armValue += factor * value.Data.Int.i;
		break;

	      case ValueSymbol:
		{
		  armValue += factor * value.Data.Symbol.offset;
		  int how;
		  switch (size)
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
  armValue = Fix_Int (size, armValue);
  if (size == 4 && swapHalfwords)
    armValue = (armValue >> 16) | (armValue << 16);
  Put_AlignDataWithOffset (offset, size, armValue, 1, !allowUnaligned);
  
  return false;
}


/**
 * Implements core of DCB, =, DCW, DCWU, &, DCD, DCDU, DCQ, DCQU, DCI.
 */
static void
DefineInt (int size, bool allowUnaligned, bool swapHalfwords, const char *mnemonic)
{
  ValueTag allowedTypes; /* Types which we accept to see in PassTwo.  */
  switch (size)
    {
      case 1:
      case 2:
      case 4:
	allowedTypes = ValueInt | ValueString; /* Only 1 byte long strings for size 2 and 4.  */
	/* Only during pass two we will emit symbols.  */
	if (gPhase != ePassOne)
	  allowedTypes |= ValueSymbol | ValueCode;
	break;

      case 8:
	/* DCQ/DCQU only supports literals.  */
	allowedTypes = ValueInt | ValueInt64;
	break;

      default:
	assert (0);
	break;
    }

  do
    {
      const uint32_t offset = areaCurrentSymbol->area->curIdx;
      const Value *valP = Expr_BuildAndEval (allowedTypes);

      bool failed = false;
      switch (valP->Tag)
	{
	  case ValueString:
	    {
	      assert (size == 1 || size == 2 || size == 4);
	      size_t len = valP->Data.String.len;
	      if (size != 1 && len != 1)
		{
		  Error (ErrorError, "Non 1 byte storage can only accept 1 byte long strings");
		  Put_AlignDataWithOffset (offset, size, 0, 1, !allowUnaligned);
		}
	      else
		{
		  const char *str = valP->Data.String.s;
		  for (size_t i = 0; i != len; ++i)
		    Put_AlignDataWithOffset (offset + i, size, (unsigned char)str[i],
					     1, !allowUnaligned);
		}
	      break;
	    }

	  case ValueInt:
	    {
	      assert (size == 1 || size == 2 || size == 4 || size == 8);
	      ARMWord armValue = size != 8 ? Fix_Int (size, valP->Data.Int.i) : (ARMWord)valP->Data.Int.i;
	      if (size == 4 && swapHalfwords)
		armValue = (armValue >> 16) | (armValue << 16);
	      Put_AlignDataWithOffset (offset, size, armValue,
				       1, !allowUnaligned);
	      break;
	    }

	  case ValueInt64:
	    {
	      assert (size == 8);
	      const uint64_t val = valP->Data.Int64.i;
	      Put_AlignDataWithOffset (offset + 0, 4, (uint32_t)val, 1, !allowUnaligned);
	      Put_AlignDataWithOffset (offset + 4, 4, (uint32_t)(val >> 32), 1, !allowUnaligned);
	      break;
	    }

	  case ValueSymbol:
	    {
	      /* Package ValueSymbol in a ValueCode.  */
	      const Code codeValueSymbol =
		{
		  .Tag = CodeValue,
		  .Data.value = *valP 
		};
	      const Value valueCode =
		{
		  .Tag = ValueCode,
		  .Data.Code =
		    {
		      .len = 1,
		      .c = &codeValueSymbol
		    }
		};
	      failed = DefineInt_HandleSymbols (size, allowUnaligned, swapHalfwords, offset, &valueCode);
	      break;
	    }

	  case ValueCode:
	    failed = DefineInt_HandleSymbols (size, allowUnaligned, swapHalfwords, offset, valP);
	    break;

	  default:
	    failed = true;
	    break;
	}
      if (failed)
	{
	  if (gPhase != ePassOne)
	    Error (ErrorError, "Illegal %s expression", mnemonic);
	  Put_AlignDataWithOffset (offset, size, 0, 1, !allowUnaligned);
	}

      Input_SkipWS ();
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
 *   {label} DCI[{.N}|{.W}] expr{,expr}
 * In ARM code: align to 4 bytes and write 4-byte value(s).
 * In Thumb code: align to 2 bytes and write 2-byte or 4-byte (when .W is
 * specified) value(s).
 */
bool
c_dci (bool doLowerCase)
{
  InstrWidth_e instrWidth = Option_GetInstrWidth (doLowerCase);
  if (instrWidth == eInstrWidth_Unrecognized)
    return true;

  InstrType_e instrState = State_GetInstrType ();
  IT_ApplyCond (areaCurrentSymbol->area->it.cc, instrState != eInstrType_ARM); 

  unsigned alignValue;
  int instructionSize;
  bool swapHalfwords;
  if (instrState == eInstrType_ARM)
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
 * Used to implement DCFS, DCFSU, DCFD and DCFDU.
 */
static void
DefineReal (int size, bool allowUnaligned, const char *mnemonic)
{
  if (Target_GetFPUFeatures () == kArchFPUExt_None)
    {
      Error (ErrorError, "No FPU selected");
      return;
    }

  do
    {
      uint32_t offset = areaCurrentSymbol->area->curIdx;

      /* FIXME? Support for ValueInt, ValueInt64 ? */
      /* FIXME: relocation support for float values ? */
      const Value *valP = Expr_BuildAndEval (ValueFloat);
      switch (valP->Tag)
	{
	  case ValueFloat:
	    Put_FloatDataWithOffset (offset, size, valP->Data.Float.f, !allowUnaligned);
	    break;

	  default:
	    if (gPhase != ePassOne)
	      Error (ErrorError, "Illegal %s expression", mnemonic);
	    Put_FloatDataWithOffset (offset, size, 0., !allowUnaligned);
	    break;
	}

      Input_SkipWS ();
    }
  while (Input_Match (',', false));
}


/**
 * Implements DCFH / DCFHU (IEEE half-precision).
 * "Define Constant Float Half-precision"
 */
bool
c_dcfh (bool doLowerCase)
{
  bool allowUnaligned = Input_Match (doLowerCase ? 'u' : 'U', false);
  if (!Input_IsEndOfKeyword ())
    return true;
  Target_CheckFPUFeature (kFPUExt_FP16, true);
  DefineReal (2, allowUnaligned, allowUnaligned ? "DCFHU" : "DCFH");
  return false;
}


/**
 * Implements DCFS / DCFSU (IEEE single-precision).
 * "Define Constant Float Single-precision"
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
 * Implements DCFD / DCFDU (IEEE double-precision).
 * "Define Constant Float Double-precision"
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
  const Value *valueTimesP = Expr_BuildAndEval (ValueInt);
  if (valueTimesP->Tag != ValueInt)
    {
      Error (ErrorError, "Unresolved reserve not possible");
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
	  const Value *valueValueP = Expr_BuildAndEval (ValueInt);
	  if (valueValueP->Tag != ValueInt)
	    {
	      Error (ErrorError, "Unresolved reserve value not possible");
	      return;
	    }
	  value = (ARMWord)valueValueP->Data.Int.i;

	  /* Check for <valuesize> presence.  */
	  if (Input_Match (',', false))
	    {
	      const Value *valueSizeP = Expr_BuildAndEval (ValueInt);
	      if (valueSizeP->Tag != ValueInt)
		{
		  Error (ErrorError, "Unresolved reserve value size not possible");
		  return;
		}
	      valueSize = (unsigned)valueSizeP->Data.Int.i;
	      if (valueSize != 1 && valueSize != 2 && valueSize != 4)
		{
		  Error (ErrorError, "Reserve value size can only be 1, 2 or 4");
		  return;
		}
	    }
	}
    }
  
  if ((int32_t)times < 0)
    Error (ErrorWarning, "Reserve space value is considered unsigned, i.e. reserving %u bytes now\n", times);

  Put_AlignDataWithOffset (areaCurrentSymbol->area->curIdx, valueSize,
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
