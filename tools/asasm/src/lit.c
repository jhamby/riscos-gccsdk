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
 * lit.c
 */

#include "config.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#include <string.h>

#include "area.h"
#include "m_cpumem.h"
#include "code.h"
#include "directive_data.h"
#include "error.h"
#include "filestack.h"
#include "m_fpe.h"
#include "help_cpu.h"
#include "lit.h"
#include "local.h"
#include "main.h"
#include "phase.h"
#include "put.h"
#include "reloc.h"
#include "value.h"

#ifdef DEBUG
//#  define DEBUG_LIT
#endif

typedef enum
{
  eNotYetAssembled,
  eNoNeedToAssemble,
  eAssembledPassOne,
  eAssembledPassTwo
} Status_e;

typedef struct LITPOOL
{
  struct LITPOOL *next;
  const char *file;	/** Assembler filename where this literal got requested for the first time.  */
  unsigned lineNum;	/** Assembler file linenumber where this literal got requested for the first time.  */

  unsigned offset;	/** Area offset where the literal got assembled.  */
  Value value;		/** Literal value.  */

  Lit_eSize size;
  Status_e status;	/** Literal's status whether it already got assembled or not.  */
} LitPool;

static Symbol *Lit_GetLitOffsetAsSymbol (const LitPool *literal);

/**
 * Returns a symbol which will be a ValueInt/ValueSymbol being the offset where the
 * given literal will be assembled.
 */
static Symbol *
Lit_GetLitOffsetAsSymbol (const LitPool *literal)
{
  char intSymbol[48];
  int bytesWritten = snprintf (intSymbol, sizeof (intSymbol), kIntLabelPrefix "Lit$%p", (void *)literal);
  assert (bytesWritten >= 0);
  const Lex lex = Lex_Id (intSymbol, (size_t)bytesWritten);
  return Symbol_Get (&lex);
}

static size_t
Lit_GetSizeInBytes (const LitPool *litP)
{
  size_t result;
  switch (litP->size)
    {
      case eLitIntUByte:
      case eLitIntSByte:
	result = 1;
	break;
      case eLitIntUHalfWord:
      case eLitIntSHalfWord:
	result = 2;
	break;
      case eLitIntWord:
      case eLitFloat:
	result = 4;
	break;
      case eLitDouble:
	result = 8;
	break;
    }
  return result;
}

static size_t
Lit_GetAlignment (const LitPool *litP)
{
  size_t result;
  switch (litP->size)
    {
      case eLitIntUByte:
      case eLitIntSByte:
	result = 1;
	break;
      case eLitIntUHalfWord:
      case eLitIntSHalfWord:
	result = 2;
	break;
      case eLitIntWord:
      case eLitFloat:
      case eLitDouble:
	result = 4;
	break;
    }
  return result;
}

/**
 * Takes ownership of valueP.
 */
static Value
Lit_CreateLiteralSymbol (const Value *valueP, Lit_eSize size)
{
  assert (valueP->Tag != ValueIllegal);

  /* We need to create a new literal in our pool.  */
  LitPool *litP;
  if ((litP = malloc (sizeof (LitPool))) == NULL)
    errorOutOfMem ();
  litP->next = NULL;
  litP->file = FS_GetCurFileName ();
  litP->lineNum = FS_GetCurLineNumber ();
  litP->offset = 0;
  litP->value = *valueP; /* We have ownership of valueP, so we're free to pass it on to LitPool struct.  */
  litP->size = size;
  litP->status = eNotYetAssembled;

  /* Add new literal at the end of the literal pool.  */
  assert (offsetof (LitPool, next) == 0);
  LitPool *prevLitP = (LitPool *)&areaCurrentSymbol->area.info->litPool;
  for (/* */; prevLitP->next != NULL; prevLitP = prevLitP->next)
    /* */;
  prevLitP->next = litP;
  
  return Value_Symbol (Lit_GetLitOffsetAsSymbol (litP), 1, 0);
}

/**
 * Registers a literal integer for the current area.
 * \return Either a ValueAddr of a previous (but same) literal integer,
 * or ValueSymbol representing the label where the literal will get assembled,
 * or ValueInt when given literal integer is representable via MOV/MVN/MOVW.
 */
Value
Lit_RegisterInt (const Value *valueP, Lit_eSize size)
{
  assert (valueP->Tag == ValueInt || valueP->Tag == ValueSymbol || valueP->Tag == ValueCode);

#ifdef DEBUG_LIT
  printf ("Lit_RegisterInt(): area offset 0x%x : type %d : ", areaCurrentSymbol->area.info->curIdx, size);
  valuePrint (valueP);
  printf ("\n");
#endif

  bool isAddrMode3;
  switch (size)
    {
      case eLitIntUByte:
      case eLitIntSByte:
      case eLitIntUHalfWord:
      case eLitIntSHalfWord:
	isAddrMode3 = true;
	break;

      case eLitIntWord:
	isAddrMode3 = false;
	break;

      default:
	assert (false && "Incorrect literal size for this routine");
    }

  /* Upfront truncate ValueInt value to what we're really going to have
     in our register after loading, before we wrongly match to the untruncated
     version for a different size value.
     E.g.  LDRB Rx, =0x808 and LDR Rx, =0x808 can not share the same literal.  */
  Value truncValue = { .Tag = ValueIllegal };
  Value_Assign (&truncValue, valueP);
  if (valueP->Tag == ValueInt)
    {
      int truncForUser;
      switch (size)
	{
	  case eLitIntUByte:
	    truncForUser = (uint8_t)truncValue.Data.Int.i;
	    truncValue.Data.Int.i &= 0xFF;
	    break;
	  case eLitIntSByte:
	    truncForUser = (int8_t)truncValue.Data.Int.i;
	    truncValue.Data.Int.i &= 0xFF;
	    break;
	  case eLitIntUHalfWord:
	    truncForUser = (uint16_t)truncValue.Data.Int.i;
	    truncValue.Data.Int.i &= 0xFFFF;
	    break;
	  case eLitIntSHalfWord:
	    truncForUser = (int16_t)truncValue.Data.Int.i;
	    truncValue.Data.Int.i &= 0xFFFF;
	    break;
	  case eLitIntWord:
	    truncForUser = truncValue.Data.Int.i;
	    break;

	  case eLitFloat:
	  case eLitDouble:
	    assert (0);
	    break;
	}
      if (truncValue.Data.Int.i != valueP->Data.Int.i)
	error (ErrorWarning, "Constant %d has been truncated to %d by the used mnemonic",
	       valueP->Data.Int.i, truncForUser);
      /* Perhaps representable as MOV/MVN/MOVW:  */
      if (Help_CPUImm8s4 (truncForUser) != UINT32_MAX
          || Help_CPUImm8s4 (~truncForUser) != UINT32_MAX
          || CPUMem_ConstantInMOVW (truncForUser))
	{
	  valueFree (&truncValue); /* Not really needed as it is ValueInt.  */
	  return Value_Int (truncForUser, eIntType_PureInt);
	}
    }
  
  /* Check if we already have the literal assembled in an range of up to
     4095 (address mode 2) or 255 (address mode 3) bytes ago.  */
  for (const LitPool *litPoolP = areaCurrentSymbol->area.info->litPool;
       litPoolP != NULL;
       litPoolP = litPoolP->next)
    {
      if (litPoolP->size == eLitFloat || litPoolP->size == eLitDouble)
	continue;

      int offset;
      bool equal;
      if (litPoolP->value.Tag == ValueInt && truncValue.Tag == ValueInt)
	{
	  switch (size)
	    {
	      case eLitIntUByte:
	      case eLitIntSByte:
		if ((((unsigned)litPoolP->value.Data.Int.i >> 0) & 0xFF) == (unsigned)truncValue.Data.Int.i)
		  {
		    offset = 0;
		    equal = true;
		  }
		else if ((((unsigned)litPoolP->value.Data.Int.i >> 8) & 0xFF) == (unsigned)truncValue.Data.Int.i)
		  {
		    offset = 1;
		    equal = true;
		  }
		else if ((((unsigned)litPoolP->value.Data.Int.i >> 16) & 0xFF) == (unsigned)truncValue.Data.Int.i)
		  {
		    offset = 2;
		    equal = true;
		  }
		else if ((((unsigned)litPoolP->value.Data.Int.i >> 24) & 0xFF) == (unsigned)truncValue.Data.Int.i)
		  {
		    offset = 3;
		    equal = true;
		  }
		else
		  equal = false;
		break;

	      case eLitIntUHalfWord:
	      case eLitIntSHalfWord:
		if ((((unsigned)litPoolP->value.Data.Int.i >> 0) & 0xFFFF) == (unsigned)truncValue.Data.Int.i)
		  {
		    offset = 0;
		    equal = true;
		  }
		else if ((((unsigned)litPoolP->value.Data.Int.i >> 16) & 0xFFFF) == (unsigned)truncValue.Data.Int.i)
		  {
		    offset = 2;
		    equal = true;
		  }
		else
		  equal = false;
		break;

	      case eLitIntWord:
		offset = 0;
		equal = litPoolP->value.Data.Int.i == truncValue.Data.Int.i;
		break;

	      default:
		equal = false;
		break;
	    }
	}
      else
	{
	  offset = 0;
	  equal = valueEqual (&litPoolP->value, &truncValue);
	}
      if (equal)
	{
	  assert (litPoolP->status != eNoNeedToAssemble);

	  Symbol *symP = Lit_GetLitOffsetAsSymbol (litPoolP);
	  assert (gPhase != ePassTwo || (symP->type & SYMBOL_DEFINED) != 0);
	  assert ((((symP->type & SYMBOL_DEFINED) == 0) && litPoolP->status == eNotYetAssembled)
	          || (((symP->type & SYMBOL_DEFINED) != 0) && (litPoolP->status == eAssembledPassOne || litPoolP->status == eAssembledPassTwo)));
	  if ((symP->type & SYMBOL_DEFINED) != 0)
	    {
	      assert (symP->value.Tag == ValueInt || symP->value.Tag == ValueSymbol);
	      assert (gPhase == ePassTwo || areaCurrentSymbol->area.info->curIdx >= litPoolP->offset);
	      if (areaCurrentSymbol->area.info->curIdx + 8 > litPoolP->offset + offset + ((isAddrMode3) ? 255 : 4095))
		continue;

	      valueFree (&truncValue);

	      /* A literal with the same value got already assembled and is in
	         our range to refer to.  */
	      return Value_Addr (15, litPoolP->offset + offset - (areaCurrentSymbol->area.info->curIdx + 8));
	    }
	  else
	    {
	      valueFree (&truncValue);

	      /* A literal with the same value was already needed so we can
	         reuse its position where it is going to be assembled.  */
	      return Value_Symbol (symP, 1, offset);
	    }
	}
    }

  assert (gPhase == ePassOne);
  return Lit_CreateLiteralSymbol (&truncValue, size);
}

/**
 * Registers a literal float for the current area.
 * \return Either a ValueAddr of a previous (but same) literal float,
 * or ValueSymbol representing the label where the literal will get assembled.
 * or ValueFloat when literal float is representable via MVF/MNF.
 */
Value
Lit_RegisterFloat (const Value *valueP, Lit_eSize size)
{
  assert (valueP->Tag == ValueInt || valueP->Tag == ValueFloat || valueP->Tag == ValueSymbol || valueP->Tag == ValueCode);
  assert ((size == eLitFloat || size == eLitDouble) && "Incorrect literal size for this routine");

#ifdef DEBUG_LIT
  printf ("Lit_RegisterFloat(): area offset 0x%x, type %d : ", areaCurrentSymbol->area.info->curIdx, size);
  valuePrint (valueP);
  printf ("\n");
#endif

  /* Convert given integer value to float.  */
  const Value value = (valueP->Tag == ValueInt) ? Value_Float ((ARMFloat)valueP->Data.Int.i) : *valueP;
  valueP = &value;

  /* Is it one of the well known FPE constants we can encode using MVF/MNF ? */
  if (valueP->Tag == ValueFloat
      && (FPE_ConvertImmediate (valueP->Data.Float.f) != (ARMWord)-1
	  || FPE_ConvertImmediate (-valueP->Data.Float.f) != (ARMWord)-1))
    return value;

  Value truncValue = { .Tag = ValueIllegal };
  Value_Assign (&truncValue, valueP);

  /* Check if we already have the literal assembled in an range of up to
     1020 bytes ago.  */
  for (const LitPool *litPoolP = areaCurrentSymbol->area.info->litPool;
       litPoolP != NULL;
       litPoolP = litPoolP->next)
    {
      if ((litPoolP->size == eLitFloat || litPoolP->size == eLitDouble)
          && litPoolP->size == size
 	  && valueEqual (&litPoolP->value, &truncValue))
	{
	  valueFree (&truncValue);

	  assert (litPoolP->status != eNoNeedToAssemble);

	  Symbol *symP = Lit_GetLitOffsetAsSymbol (litPoolP);
	  assert (gPhase != ePassTwo || (symP->type & SYMBOL_DEFINED) != 0);
	  assert ((((symP->type & SYMBOL_DEFINED) == 0) && litPoolP->status == eNotYetAssembled)
	          || (((symP->type & SYMBOL_DEFINED) != 0) && (litPoolP->status == eAssembledPassOne || litPoolP->status == eAssembledPassTwo)));
	  if ((symP->type & SYMBOL_DEFINED) != 0)
	    {
	      assert (symP->value.Tag == ValueFloat || symP->value.Tag == ValueSymbol);
	      assert (gPhase == ePassTwo || areaCurrentSymbol->area.info->curIdx >= litPoolP->offset);
	      if (areaCurrentSymbol->area.info->curIdx + 8 > litPoolP->offset + 1020)
		continue;
	      /* A literal with the same value got already assembled and is in
	         our range to refer to.  */
	      return Value_Addr (15, litPoolP->offset - (areaCurrentSymbol->area.info->curIdx + 8));
	    }
	  else
	    {
	      /* A literal with the same value was already needed so we can
	         reuse its position where it is going to be assembled.  */
	      return Value_Symbol (symP, 1, 0);
	    }
	}
    }

  assert (gPhase == ePassOne);
  return Lit_CreateLiteralSymbol (&truncValue, size);
}

/**
 * Dump (assemble) the literal pool of the current area.
 */
void
Lit_DumpPool (void)
{
#ifdef DEBUG_LIT
  printf ("Lit_DumpPool(), area offset 0x%x\n", areaCurrentSymbol->area.info->curIdx);
#endif

  Status_e statusToLeaveAlone = (gPhase == ePassOne) ? eAssembledPassOne : eAssembledPassTwo;
  for (LitPool *litP = areaCurrentSymbol->area.info->litPool; litP != NULL; litP = litP->next)
    {
      if (litP->status == statusToLeaveAlone || litP->status == eNoNeedToAssemble)
	continue;
      assert ((gPhase == ePassOne && litP->status == eNotYetAssembled) || (gPhase == ePassTwo && litP->status == eAssembledPassOne));

      const size_t alignValue = Lit_GetAlignment (litP);
      if (gPhase == ePassTwo && litP->offset > ((areaCurrentSymbol->area.info->curIdx + alignValue-1) & -alignValue))
	break;

      litP->status = gPhase == ePassOne ? eAssembledPassOne : eAssembledPassTwo;
      
      /* Re-evaluate symbol/code.  It might be resolvable by now.  */
      if (litP->value.Tag == ValueSymbol || litP->value.Tag == ValueCode)
	{
	  codeInit ();
	  codeValue (&litP->value, true);
	  const Value *constValueP = codeEval (ValueInt | ValueCode | ValueSymbol | ValueFloat, NULL);
	  if (constValueP->Tag == ValueIllegal)
	    {
	      errorLine (litP->file, litP->lineNum, ErrorError, "Unsupported literal case");
	      continue;
	    }
	  /* Upgrade ValueInt to ValueFloat when we have a float/double literal.  */
          const Value dblValue = (constValueP->Tag == ValueInt
	                          && (litP->size == eLitFloat || litP->size == eLitDouble)) ? Value_Float ((ARMFloat)constValueP->Data.Int.i) : *constValueP;
	  Value_Assign (&litP->value, &dblValue);
	}

      Symbol *symP = Lit_GetLitOffsetAsSymbol (litP);
      symP->type |= SYMBOL_DEFINED;
      symP->area.rel = areaCurrentSymbol;

      /* Check if it is a fixed integer/float which fits an immediate
         representation.  */
      switch (litP->value.Tag)
	{
	  case ValueInt:
	    {
	      bool isImmediate;
	      int constant;
	      switch (litP->size)
		{
		  case eLitIntUByte:
		    constant = (int)(uint8_t)litP->value.Data.Int.i;
		    isImmediate = true;
		    break;

		  case eLitIntSByte:
		    constant = (int)(int8_t)litP->value.Data.Int.i;
		    isImmediate = true;
		    break;

		  case eLitIntUHalfWord:
		    constant = (int)(uint16_t)litP->value.Data.Int.i;
		    isImmediate = true;
		    break;

		  case eLitIntSHalfWord:
		    constant = (int)(int16_t)litP->value.Data.Int.i;
		    isImmediate = true;
		    break;

		  case eLitIntWord:
		    constant = litP->value.Data.Int.i;
		    isImmediate = true;
		    break;

		  default:
		    isImmediate = false;
		    break;
		}
	      if (constant != litP->value.Data.Int.i)
		errorLine (litP->file, litP->lineNum, ErrorWarning,
			   "Constant %d has been truncated to %d by the used mnemonic",
			   litP->value.Data.Int.i, constant);

	      /* Value representable using MOV or MVN ? */
	      if (isImmediate
		  && (Help_CPUImm8s4 (constant) != UINT32_MAX
		      || Help_CPUImm8s4 (~constant) != UINT32_MAX
		      || CPUMem_ConstantInMOVW (constant)))
		{
		  if (gPhase == ePassOne)
		    {
		      /* The definition of the literal happeded after its use
			 but before LTORG so we don't have to assemble it
			 explicitely.  */
		      symP->value = Value_Int (constant, eIntType_PureInt);
		      litP->status = eNoNeedToAssemble;
		      continue;
		    }
		  else
		    {
		      /* We do MOV/MVN/MOVW optimisation but as the literal value
		         got defined after LTORG, we've already allocated
		         some bytes which aren't going to be used.  */
		      errorLine (litP->file, litP->lineNum, ErrorWarning,
			         "Literal loading optimized as MOV/MVN/MOVW but because of literal value definition after LTORG this results in %zd bytes waste", Lit_GetSizeInBytes (litP));
		      error (ErrorWarning, "note: LTORG was here");
		    }
		}
	      break;
	    }

	  case ValueCode:
	  case ValueSymbol:
	    {
	      /* Case:          LDR Rx, =<label>
	               <label>  ...  */
	      break;
	    }

	  case ValueFloat:
	    {
	      /* Value representable using MVF or MNF ? */
	      ARMFloat constant = litP->value.Data.Float.f;
	      if (FPE_ConvertImmediate (constant) != (ARMWord)-1
		  || FPE_ConvertImmediate (-constant) != (ARMWord)-1)
		{
		  if (gPhase == ePassOne)
		    {
		      /* The definition of the literal happeded after its use
			 but before LTORG so we don't have to assemble it
			 explicitely.  */
		      symP->value = Value_Float (constant);
		      litP->status = eNoNeedToAssemble;
		      continue;
		    }
		  else
		    {
		      /* We do MVF/MNF optimisation but as the literal value
			 got defined after LTORG, we've already allocated
			 some bytes which aren't going to be used.  */
		      errorLine (litP->file, litP->lineNum, ErrorWarning,
				 "Literal loading optimized as MVF/MNF but because of literal value definition after LTORG this results in %zd bytes waste", Lit_GetSizeInBytes (litP));
		      error (ErrorWarning, "note: LTORG was here");
		    }
		}
	      break;
	    }
	    
	  default:
	    errorLine (litP->file, litP->lineNum, ErrorError, "Unsupported literal case");
	    break;
	}

      /* At this point we're sure we're going to write data in the current
         area.  Mark it as such.  */
      Area_MarkStartAs (areaCurrentSymbol, areaCurrentSymbol->area.info->curIdx, eData);
      
      /* Ensure alignment.  */
      switch (litP->size)
	{
	  case eLitIntUHalfWord:
	  case eLitIntSHalfWord:
	    Area_AlignArea (areaCurrentSymbol, 2, NULL);
	    break;

	  case eLitIntWord:
	  case eLitFloat:
	  case eLitDouble:
	    Area_AlignArea (areaCurrentSymbol, 4, NULL);
	    break;

	  default:
	    break;
	}
      
      /* The constant is not representable as an Immediate constant.  So we
         have to assemble it and refer to it via a ValueSymbol(current area)
	 + ValueInt(offset where literal gets assembled).
	 The ValueSymbol will be translated to a ValueAddr(pc) at a later
	 stage.  */
      Value newSymValue = Value_Symbol (areaCurrentSymbol, 1, areaCurrentSymbol->area.info->curIdx);
      Value_Assign (&symP->value, &newSymValue);

      assert ((gPhase == ePassOne && litP->offset == 0) || (gPhase == ePassTwo && litP->offset == areaCurrentSymbol->area.info->curIdx));
      litP->offset = areaCurrentSymbol->area.info->curIdx;

      switch (litP->size)
	{
	  case eLitIntUByte:
	  case eLitIntSByte:
	  case eLitIntUHalfWord:
	  case eLitIntSHalfWord:
	  case eLitIntWord:
	    {
#ifdef DEBUG_LIT
	      printf ("  Place at 0x%x value ", litP->offset);
	      valuePrint (&litP->value);
	      printf ("\n");
#endif
	      size_t litSize = Lit_GetSizeInBytes (litP);
	      if (gPhase == ePassOne)
		Put_AlignDataWithOffset (litP->offset, litSize, 0, 1, true);
	      else
		{
		  /* Package our literal value in a ValueCode.  */
		  const Code codeValueData =
		    {
		      .Tag = CodeValue,
		      .Data.value = litP->value
		    };
		  const Value valueCode =
		    {
		      .Tag = ValueCode,
		      .Data.Code =
			{
			  .len = 1,
			  .c = &codeValueData
			}
		    };
		  if (DefineInt_HandleSymbols (litSize, false, false, litP->offset, litP->value.Tag == ValueCode ? &litP->value : &valueCode))
		    {
		      errorLine (litP->file, litP->lineNum, ErrorError, "Illegal %s expression", "literal");
		      Put_AlignDataWithOffset (litP->offset, litSize, 0, 1, true);
		    }
		}
	      break;
	    }

	  case eLitFloat:
	  case eLitDouble:
	    {
#ifdef DEBUG_LIT
	      printf ("  Place at 0x%x value ", litP->offset);
	      valuePrint (&litP->value);
	      printf ("\n");
#endif
	      double litData;
	      if (gPhase == ePassOne)
		litData = 0.;
	      else
		{
		  /* Final evaluation.  */
		  codeInit ();
		  codeValue (&litP->value, true);
		  const Value *valP = codeEval (ValueFloat | ValueSymbol, NULL);

		  /* FIXME: relocation support for float values ? */
		  if (valP->Tag == ValueFloat)
		    litData = valP->Data.Float.f;
		  else
		    {
		      errorLine (litP->file, litP->lineNum, ErrorError, "Illegal %s expression", "literal");
		      litData = 0.;
		    }
		}
	      Put_FloatDataWithOffset (litP->offset, Lit_GetSizeInBytes (litP), litData, true);
	      break;
	    }
	}
    }

  Area_AlignArea (areaCurrentSymbol, 4, NULL);
}


/**
 * Implements LTORG.
 */
bool
c_ltorg (void)
{
  Lit_DumpPool ();
  return false;
}
