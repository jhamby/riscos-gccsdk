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

#include "area.h"
#include "code.h"
#include "directive_data.h"
#include "error.h"
#include "expr.h"
#include "fix.h"
#include "input.h"
#include "phase.h"
#include "put.h"


/**
 * Implements ALIGN [<power-of-2> [, <offset>]]
 */
Rslt_e
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

  Area_EnsureExtraSize (areaCurrentSymbol, bytesToStuff);

  while (bytesToStuff--)
    areaCurrentSymbol->area.info->image[areaCurrentSymbol->area.info->curIdx++] = 0;

  return eRslt_None;
}

/**
 * Implements DATA (as nop).
 */
Rslt_e
c_data (void)
{
  return eRslt_None;
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
	Put_AlignDataWithOffset (offset + i, privDataP->size, str[i], 1,
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
	      && valueP->Data.Code.c[i + 1].Data.op == Op_sub)
	    factor = -1;
	  else
	    factor = 1;

	  const Code *codeP = &valueP->Data.Code.c[i];
	  if (codeP->Tag == CodeOperator)
	    {
	      if (codeP->Data.op != Op_add && codeP->Data.op != Op_sub)
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
	      && valueP->Data.Code.c[i + 1].Data.op == Op_sub)
	    factor = -1;
	  else
	    factor = 1;

	  const Code *codeP = &valueP->Data.Code.c[i];
	  if (codeP->Tag == CodeOperator)
	    {
	      if (codeP->Data.op != Op_add && codeP->Data.op != Op_sub)
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
  Put_AlignDataWithOffset (offset, privDataP->size, armValue, 1,
                           !privDataP->allowUnaligned);
  
  return false;
}

static void
DefineInt (int size, bool allowUnaligned, const char *mnemonic)
{
  DefineInt_PrivData_t privData =
    {
      .size = size,
      .allowUnaligned = allowUnaligned
    };
  do
    {
      exprBuild ();
      if (gPhase == ePassOne)
	{
	  const Value *result = codeEval (ValueInt | ValueString | ValueSymbol | ValueCode, NULL);
	  if (result->Tag == ValueString)
	    {
	      size_t len = result->Data.String.len;
	      const char *str = result->Data.String.s;
	      /* Lay out a string.  */
	      for (size_t i = 0; i != len; ++i)
		Put_AlignDataWithOffset (areaCurrentSymbol->area.info->curIdx,
		                         privData.size, str[i], 1,
		                         !privData.allowUnaligned);
	    }
	  else
	    Put_AlignDataWithOffset (areaCurrentSymbol->area.info->curIdx,
	                             privData.size, 0, 1,
	                             !privData.allowUnaligned);
	}
      else if (Reloc_QueueExprUpdate (DefineInt_RelocUpdater, areaCurrentSymbol->area.info->curIdx,
				      ValueInt | ValueString | ValueSymbol | ValueCode,
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
Rslt_e
c_dcb (void)
{
  DefineInt (1, true, "DCB or =");
  return eRslt_Data;
}

/**
 * Implements DCW, DCWU (16 bit integer).
 * "Define Constant Word"
 */
Rslt_e
c_dcw (bool doLowerCase)
{
  bool allowUnaligned = Input_Match (doLowerCase ? 'u' : 'U', false);
  if (!Input_IsEndOfKeyword ())
    return eRslt_NotRecognized;
  DefineInt (2, allowUnaligned, allowUnaligned ? "DCWU" : "DCW");
  return eRslt_Data;
}

/**
 * Implements & (32 bit integer).
 * "Define Constant Double-word"
 */
Rslt_e
c_ampersand (void)
{
  DefineInt (4, false, "&");
  return eRslt_Data;
}

/**
 * Implements DCD and DCDU (32 bit integer).
 * "Define Constant Double-word"
 */
Rslt_e
c_dcd (bool doLowerCase)
{
  bool allowUnaligned = Input_Match (doLowerCase ? 'u' : 'U', false);
  if (!Input_IsEndOfKeyword ())
    return eRslt_NotRecognized;
  DefineInt (4, allowUnaligned, allowUnaligned ? "DCDU" : "DCD");
  return eRslt_Data;
}

/**
 * Implements DCI.
 * "Define Constant Instruction"
 */
Rslt_e
c_dci (void)
{
  Area_AlignArea (areaCurrentSymbol, 4, "instruction");
  DefineInt (4, false, "DCI");
  return eRslt_ARM; /* FIXME: this is arbitrary, could be eRslt_Thumb as well.  */
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
	  if (codeP->Data.op != Op_add)
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
Rslt_e
c_dcfs (bool doLowerCase)
{
  bool allowUnaligned = Input_Match (doLowerCase ? 'u' : 'U', false);
  if (!Input_IsEndOfKeyword ())
    return eRslt_NotRecognized;
  DefineReal (4, allowUnaligned, allowUnaligned ? "DCFSU" : "DCFS");
  return eRslt_Data;
}

/**
 * Implements DCFD / DCFDU (IEEE Double Precision).
 * "Define Constant Float-double precision"
 */
Rslt_e
c_dcfd (bool doLowerCase)
{
  bool allowUnaligned = Input_Match (doLowerCase ? 'u' : 'U', false);
  if (!Input_IsEndOfKeyword ())
    return eRslt_NotRecognized;
  DefineReal (8, allowUnaligned, allowUnaligned ? "DCFDU" : "DCFD");
  return eRslt_Data;
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
Rslt_e
c_fill (void)
{
  ReserveSpace (true);
  return eRslt_Data;
}


/**
 * Implements '%', 'SPACE'.
 *   {label} % <expr>
 *   {label} SPACE <expr>
 */
Rslt_e
c_reserve (void)
{
  ReserveSpace (false);
  return eRslt_Data;
}
