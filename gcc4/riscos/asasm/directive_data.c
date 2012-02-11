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
	Put_AlignDataWithOffset (offset + i, privDataP->size, str[i], !privDataP->allowUnaligned);
      return false;
    }

  if (!final)
    {
      Put_AlignDataWithOffset (offset, privDataP->size, 0, !privDataP->allowUnaligned);
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
  Put_AlignDataWithOffset (offset, privDataP->size, armValue, !privDataP->allowUnaligned);
  
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
		Put_AlignDataWithOffset (areaCurrentSymbol->area.info->curIdx, privData.size, str[i], !privData.allowUnaligned);
	    }
	  else
	    Put_AlignDataWithOffset (areaCurrentSymbol->area.info->curIdx, privData.size, 0, !privData.allowUnaligned);
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
