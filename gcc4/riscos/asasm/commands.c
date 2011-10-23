/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 1997 Darren Salt
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
 * commands.c
 */

#include "config.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#include <math.h>

#include "area.h"
#include "code.h"
#include "commands.h"
#include "error.h"
#include "expr.h"
#include "filestack.h"
#include "fix.h"
#include "include.h"
#include "input.h"
#include "lex.h"
#include "local.h"
#include "main.h"
#include "os.h"
#include "output.h"
#include "put.h"
#include "symbol.h"
#include "value.h"

/**
 * Define given symbol with what's located at the parser.  Fail when the
 * symbol is already defined with a value different than parsed.
 */
static bool
Define (const char *msg, Symbol *sym, unsigned symType, ValueTag legal)
{
  bool fail;
  if (sym == NULL)
    {
      error (ErrorError, "Missing label before %s", msg);
      fail = true;
    }
  else
    {
      const Value *value = exprBuildAndEval (legal);
      if (value->Tag == ValueIllegal)
	{
	  error (ErrorError, "Illegal %s", msg);
	  fail = true;
	}
      else
	fail = Symbol_Define (sym, SYMBOL_ABSOLUTE | symType, value);
    }
  return fail;
}

/**
 * Implements EQU and *.
 */
bool
c_equ (Symbol *symbol)
{
  Define ("* or EQU", symbol, 0, ValueAll);
  return false;
}

/**
 * Implements FN.
 */
bool
c_fn (Symbol *symbol)
{
  if (!Define ("float register", symbol, SYMBOL_FPUREG, ValueInt))
    {
      int no = symbol->value.Data.Int.i;
      if (no < 0 || no > 7)
	{
	  symbol->value.Data.Int.i = 0;
	  error (ErrorError, "Illegal %s register %d (using 0)", "FPU", no);
	}
    }
  return false;
}

/**
 * Implements RN.
 */
bool
c_rn (Symbol *symbol)
{
  if (!Define ("register", symbol, SYMBOL_CPUREG, ValueInt))
    {
      int no = symbol->value.Data.Int.i;
      if (no < 0 || no > 15)
	{
	  symbol->value.Data.Int.i = 0;
	  error (ErrorError, "Illegal %s register %d (using 0)", "CPU", no);
	}
    }
  return false;
}

/**
 * Implements CN.
 */
bool
c_cn (Symbol *symbol)
{
  if (!Define ("coprocessor register", symbol, SYMBOL_COPREG, ValueInt))
    {
      int no = symbol->value.Data.Int.i;
      if (no < 0 || no > 15)
	{
	  symbol->value.Data.Int.i = 0;
	  error (ErrorError, "Illegal %s register %d (using 0)", "coprocessor", no);
	}
    }
  return false;
}

/**
 * Implements CP.
 */
bool
c_cp (Symbol *symbol)
{
  if (!Define ("coprocessor number", symbol, SYMBOL_COPNUM, ValueInt))
    {
      int no = symbol->value.Data.Int.i;
      if (no < 0 || no > 15)
	{
	  symbol->value.Data.Int.i = 0;
	  error (ErrorError, "Illegal coprocessor number %d (using 0)", no);
	}
    }
  return false;
}

/**
 * Implements "HEAD" : APCS function name signature.
 * ObjAsm extension.
 */
bool
c_head (void)
{
  const unsigned startAreaOffset = areaCurrentSymbol->area.info->curIdx;
  const Value *value = exprBuildAndEval (ValueString);
  switch (value->Tag)
    {
      case ValueString:
	{
	  size_t len = value->Data.String.len;
	  const char *str = value->Data.String.s;
	  for (size_t i = 0; i < len; ++i)
	    Put_Data (1, str[i]);
	  Put_Data (1, '\0');
	}
        break;

      default:
        error (ErrorError, "Illegal %s expression", "string");
        break;
    }

  Area_AlignTo (areaCurrentSymbol->area.info->curIdx, 4, NULL);
  Put_Data (4, 0xFF000000 + areaCurrentSymbol->area.info->curIdx - startAreaOffset);
  return false;
}


/**
 * Reloc updater for DefineInt().
 */
bool
DefineInt_RelocUpdater (const char *file, int lineno, ARMWord offset,
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
	    {
	      ARMWord word = Fix_Int (file, lineno, privDataP->size, valP->Data.Int.i);
	      Put_AlignDataWithOffset (offset, privDataP->size, word, !privDataP->allowUnaligned);
	    }
	    break;

	  case ValueSymbol:
	    {
	      if (!final)
		{
		  Put_AlignDataWithOffset (offset, privDataP->size, 0, !privDataP->allowUnaligned);
		  return true;
		}
	      int How;
	      switch (privDataP->size)
		{
		  case 1:
		    How = HOW2_INIT | HOW2_BYTE;
		    break;
		  case 2:
		    How = HOW2_INIT | HOW2_HALF;
		    break;
		  case 4:
		    How = HOW2_INIT | HOW2_WORD;
		    break;
		  default:
		    assert (0);
		    break;
		}
	      if (Reloc_Create (How, offset, valP) == NULL)
		return true;
	    }
	    break;

	  default:
	    return true;
	}
    }
  
  return false;
}

static bool
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
      if (gASM_Phase == ePassOne)
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
  return false;
}

/**
 * Implements DCB and = (8 bit integer).
 * "Define Constant Byte"
 */
bool
c_dcb (void)
{
  return DefineInt (1, true, "DCB or =");
}

/**
 * Implements DCW, DCWU (16 bit integer).
 * "Define Constant Word"
 */
bool
c_dcw (void)
{
  bool allowUnaligned = Input_Match ('U', false);
  if (inputLook () && !isspace ((unsigned char)inputLook ()))
    return true;
  return DefineInt (2, allowUnaligned, allowUnaligned ? "DCWU" : "DCW");
}

/**
 * Implements DCD, DCDU and & (32 bit integer).
 * "Define Constant Double-word"
 */
bool
c_dcd (void)
{
  bool allowUnaligned = Input_Match ('U', false);
  if (inputLook () && !isspace ((unsigned char)inputLook ()))
    return true;
  return DefineInt (4, allowUnaligned, allowUnaligned ? "DCDU" : "DCD");
}

/**
 * Implements DCI.
 * "Define Constant Instruction"
 */
bool
c_dci (void)
{
  Area_AlignTo (areaCurrentSymbol->area.info->curIdx, 4, "instruction");
  return DefineInt (4, false, "DCI");
}

/**
 * Reloc updater for DefineReal().
 */
bool
DefineReal_RelocUpdater (const char *file, int lineno, ARMWord offset,
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
	    errorLine (file, lineno, ErrorError, "Can't create relocation");
	    break;

	  default:
	    return true;
	}
    }
  
  return false;
}

static bool
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
      if (gASM_Phase == ePassOne)
	Put_FloatDataWithOffset (areaCurrentSymbol->area.info->curIdx,
				 privData.size, 0., !privData.allowUnaligned);
      else
	{
	  ValueTag legal = ValueFloat | ValueSymbol | ValueCode;
	  if (option_autocast)
	    legal |= ValueInt;
	  if (Reloc_QueueExprUpdate (DefineReal_RelocUpdater, areaCurrentSymbol->area.info->curIdx,
				     legal, &privData, sizeof (privData)))
	    error (ErrorError, "Illegal %s expression", mnemonic);
	}
      
      skipblanks ();
    }
  while (Input_Match (',', false));
  return false;
}

/**
 * Implements DCFS / DCFSU (IEEE Single Precision).
 * "Define Constant Float-single precision"
 */
bool
c_dcfs (void)
{
  bool allowUnaligned = Input_Match ('U', false);
  if (inputLook () && !isspace ((unsigned char)inputLook ()))
    return true;
  return DefineReal (4, allowUnaligned, allowUnaligned ? "DCFSU" : "DCFS");
}

/**
 * Implements DCFD / DCFDU (IEEE Double Precision).
 * "Define Constant Float-double precision"
 */
bool
c_dcfd (void)
{
  bool allowUnaligned = Input_Match ('U', false);
  if (inputLook () && !isspace ((unsigned char)inputLook ()))
    return true;
  return DefineReal (8, allowUnaligned, allowUnaligned ? "DCFDU" : "DCFD");
}

/**
 * Called for GET / INCLUDE
 */
bool
c_get (void)
{
  char *filename;
  if ((filename = strdup (Input_Rest ())) == NULL)
    errorOutOfMem ();
  char *cptr;
  for (cptr = filename; *cptr && !isspace ((unsigned char)*cptr); cptr++)
    /* */;
  if (*cptr)
    {
      *cptr++ = '\0'; /* must be a space */
      while (*cptr && isspace ((unsigned char)*cptr))
	cptr++;
      if (*cptr && *cptr != ';')
	error (ErrorError, "Skipping extra characters '%s' after filename", cptr);
    }

  if (!FS_PushFilePObject (filename) && option_verbose)
    fprintf (stderr, "Including file \"%s\" as \"%s\"\n", filename, gCurPObjP->name);
  return false;
}

/**
 * Implements LNK.
 */
bool
c_lnk (void)
{
  char *filename;
  if ((filename = strdup (Input_Rest ())) == NULL)
    errorOutOfMem ();
  char *cptr;
  for (cptr = filename; *cptr && !isspace ((unsigned char)*cptr); cptr++)
    /* */;
  if (*cptr)
    {
      *cptr++ = '\0'; /* must be a space */
      while (*cptr && isspace ((unsigned char)*cptr))
	cptr++;
      if (*cptr && *cptr != ';')
	error (ErrorError, "Skipping extra characters '%s' after filename", cptr);
    }

  /* Terminate all outstanding macro calls and finish the current file.  */
  while (gCurPObjP->type == POType_eMacro)
    FS_PopPObject (true);
  FS_PopPObject (true);

  if (!FS_PushFilePObject (filename) && option_verbose)
    fprintf (stderr, "Linking to file \"%s\" as \"%s\"\n", filename, gCurPObjP->name);
  return false;
}

/**
 * Implements IDFN.
 */
bool
c_idfn (void)
{
  free ((void *)idfn_text);
  if ((idfn_text = strdup (Input_Rest ())) == NULL)
    errorOutOfMem();
  return false;
}

/**
 * Implements BIN / INCBIN.
 */
bool
c_incbin (void)
{
  char *filename;
  if ((filename = strdup (Input_Rest ())) == NULL)
    errorOutOfMem ();
  char *cptr;
  for (cptr = filename; *cptr && !isspace ((unsigned char)*cptr); cptr++)
    /* */;
  *cptr = '\0';

  const char *newFilename;
  FILE *binfp = Include_Get (filename, &newFilename, true);
  if (!binfp)
    {
      error (ErrorError, "Cannot open file \"%s\"", filename);
      free (filename);
      free ((void *)newFilename);
      return false;
    }
  if (option_verbose)
    fprintf (stderr, "Including binary file \"%s\" as \"%s\"\n", filename, newFilename);
  free ((void *)newFilename);
  /* Include binary file.  */
  int c;
  while ((c = getc (binfp)) != EOF)
    Put_Data (1, c);
  fclose (binfp);
  return false;
}

/**
 * Implements END.
 */
bool
c_end (void)
{
  if (gCurPObjP->type == POType_eMacro)
    error (ErrorError, "Cannot use END within a macro");
  else
    FS_PopPObject (false);
  return false;
}

/**
 * Implements ASSERT.
 */
bool
c_assert (void)
{
  if (gASM_Phase == ePassOne)
    Input_Rest ();
  else
    {
      const Value *value = exprBuildAndEval (ValueBool);
      switch (value->Tag)
	{
	  case ValueBool:
	    if (!value->Data.Bool.b)
	      error (ErrorError, "Assertion failed");
	    break;

	  default:
	    error (ErrorError, "ASSERT expression must be boolean");
	    break;
	}
    }
  
  return false;
}

/**
 * Implementation for:
 *   ! <arithmetic expression>, <string expression>
 *   INFO <arithmetic expression>, <string expression>
 *
 * When <arithmetic expression> evaluates to 0, <string expression> is
 * outputed as is.  When it evaluates to non-0, <string expression> is given
 * as error.
 */
bool
c_info (void)
{
  const Value *value = exprBuildAndEval (ValueInt | ValueFloat);
  if (value->Tag != ValueInt && value->Tag != ValueFloat)
    {
      error (ErrorError, "INFO expression must be arithmetic");
      return false;
    }
  bool giveErr = (value->Tag == ValueInt && value->Data.Int.i != 0)
		   || (value->Tag == ValueFloat && fabs (value->Data.Float.f) >= 0.00001);

  skipblanks();
  if (!Input_Match (',', false))
    {
      error (ErrorError, "Missing , in INFO directive");
      return false;
    }

  const Value *message = exprBuildAndEval (ValueString);
  if (message->Tag != ValueString)
    {
      error (ErrorError, "INFO message must be a string");
      return false;
    }

  /* Give output during pass one.  */
  if (gASM_Phase == ePassOne)
    {
      if (giveErr)
	error (ErrorError, "%.*s", (int)message->Data.String.len, message->Data.String.s);
      else
	printf ("%.*s\n", (int)message->Data.String.len, message->Data.String.s);
    }
  return false;
}

/**
 * Implements OPT.
 */
bool
c_opt (void)
{
  Input_Rest ();
  /* Do nothing.  This is for compatiblity with objasm.  */
  return false;
}

/**
 * Implements SUBT (subtitle) / TTL (title).
 */
bool
c_title (void)
{
  Input_Rest ();
  /* Do nothing right now.  This command is for the benefit of error reporting */
  return false;
}
