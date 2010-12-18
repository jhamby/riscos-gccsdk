/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 1997 Darren Salt
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

static void
c_define (const char *msg, Symbol *sym, ValueTag legal)
{
  if (!sym)
    errorAbort ("Missing label before %s", msg);
  sym->type |= SYMBOL_ABSOLUTE;
  const Value *value = exprBuildAndEval (legal);
  if (value->Tag == ValueIllegal)
    {
      error (ErrorError, "Illegal %s", msg);
      sym->value = Value_Int (0);
    }
  else
    Value_Assign (&sym->value, value);
  sym->type |= SYMBOL_DEFINED | SYMBOL_DECLARED;
}

/**
 * Implements EQU and *.
 */
bool
c_equ (Symbol *symbol)
{
  c_define ("EQU", symbol, ValueAll);
  return false;
}

/**
 * Implements FN.
 */
bool
c_fn (Symbol *symbol)
{
  c_define ("float register", symbol, ValueInt);
  symbol->type |= SYMBOL_FPUREG;
  int no = symbol->value.Data.Int.i;
  if (no < 0 || no > 7)
    {
      symbol->value.Data.Int.i = 0;
      error (ErrorError, "Illegal %s register %d (using 0)", "FPU", no);
    }
  return false;
}

/**
 * Implements RN.
 */
bool
c_rn (Symbol *symbol)
{
  c_define ("register", symbol, ValueInt);
  symbol->type |= SYMBOL_CPUREG;
  int no = symbol->value.Data.Int.i;
  if (no < 0 || no > 15)
    {
      symbol->value.Data.Int.i = 0;
      error (ErrorError, "Illegal %s register %d (using 0)", "CPU", no);
    }
  return false;
}

/**
 * Implements CN.
 */
bool
c_cn (Symbol *symbol)
{
  c_define ("coprocessor register", symbol, ValueInt);
  symbol->type |= SYMBOL_COPREG;
  int no = symbol->value.Data.Int.i;
  if (no < 0 || no > 15)
    {
      symbol->value.Data.Int.i = 0;
      error (ErrorError, "Illegal %s register %d (using 0)", "coprocessor", no);
    }
  return false;
}

/**
 * Implements CP.
 */
bool
c_cp (Symbol *symbol)
{
  c_define ("coprocessor number", symbol, ValueInt);
  symbol->type |= SYMBOL_COPNUM;
  int no = symbol->value.Data.Int.i;
  if (no < 0 || no > 15)
    {
      symbol->value.Data.Int.i = 0;
      error (ErrorError, "Illegal coprocessor number %d (using 0)", no);
    }
  return false;
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

/**
 * Implements "HEAD" : APCS function name signature.
 * ObjAsm extension.
 */
bool
c_head (void)
{
  skipblanks ();
  const Value *value = exprBuildAndEval (ValueString);
  switch (value->Tag)
    {
      case ValueString:
	{
	  size_t len = value->Data.String.len;
	  const char *str = value->Data.String.s;
	  for (size_t i = 0; i < len; ++i)
	    putData (1, str[i]);
	  putData (1, 0);
	}
        break;
      default:
        error (ErrorError, "Illegal %s expression", "string");
        break;
    }

  /* Align.  */
  int i = areaCurrentSymbol->value.Data.Int.i;
  while (areaCurrentSymbol->value.Data.Int.i & 3)
    areaCurrentSymbol->area.info->image[areaCurrentSymbol->value.Data.Int.i++] = 0;
  putData (4, 0xFF000000 + (areaCurrentSymbol->value.Data.Int.i - i));
  return false;
}

/**
 * Reloc updater for DefineInt().
 */
bool
DefineInt_RelocUpdater (const char *file, int lineno, ARMWord offset,
			const Value *valueP, void *privData, bool final)
{
  const int size = *(int *)privData;

  assert (valueP->Tag == ValueCode && valueP->Data.Code.len != 0);

  /* ValueString for size = 1 is an odd one case:  */
  if (!final
      && size == 1
      && valueP->Data.Code.len == 1
      && valueP->Data.Code.c[0].Tag == CodeValue
      && valueP->Data.Code.c[0].Data.value.Tag == ValueString)
    {
      size_t len = valueP->Data.Code.c[0].Data.value.Data.String.len;
      const char *str = valueP->Data.Code.c[0].Data.value.Data.String.s;
      /* Lay out a string.  */
      for (size_t i = 0; i != len; ++i)
	Put_DataWithOffset (offset + i, 1, str[i]);
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
	      ARMWord word = Fix_Int (file, lineno, size, valP->Data.Int.i);
	      Put_DataWithOffset (offset, size, word);
	    }
	    break;

	  case ValueSymbol:
	    {
	      if (!final)
		{
		  Put_DataWithOffset (offset, size, 0);
		  return true;
		}
#if 0 /* FIXME: is this needed ? */
	      if (size != 4)
		{
		  errorLine (file, lineno, ErrorError, "Wrong data size for relocation");
		  return true;
		}
#endif
	      int How;
	      switch (size)
		{
		  case 4:
		    How = HOW2_INIT | HOW2_WORD;
		    break;
		    case 2: // FIXME: needed ?
		      How = HOW2_INIT | HOW2_HALF;
		    break;
		    case 1: // FIXME: needed ?
		      How = HOW2_INIT | HOW2_BYTE;
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
DefineInt (int size, const char *mnemonic)
{
  do
    {
      exprBuild ();
      if (Reloc_QueueExprUpdate (DefineInt_RelocUpdater, areaCurrentSymbol->value.Data.Int.i,
				 ValueInt | ValueString | ValueSymbol | ValueCode,
				 &size, sizeof (size)))
	error (ErrorError, "Illegal %s expression", mnemonic);

      skipblanks ();
    }
  while (Input_Match (',', false));
  return false;
}

/**
 * Implements DCB and = (8 bit integer).
 */
bool
c_dcb (void)
{
  return DefineInt (1, "DCB or =");
}

/**
 * Implements DCW (16 bit integer).
 */
bool
c_dcw (void)
{
  return DefineInt (2, "DCW");
}

/**
 * Implements DCD and & (32 bit integer).
 */
bool
c_dcd (void)
{
  return DefineInt (4, "DCD");
}

/**
 * Reloc updater for DefineReal().
 */
bool
DefineReal_RelocUpdater (const char *file, int lineno, ARMWord offset,
			 const Value *valueP, void *privData, bool final)
{
  const int size = *(int *)privData;

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
	    Put_FloatDataWithOffset (offset, size, valP->Data.Int.i);
	    break;

	  case ValueFloat:
	    Put_FloatDataWithOffset (offset, size, valP->Data.Float.f);
	    break;

	  case ValueSymbol:
	    if (!final)
	      {
		Put_FloatDataWithOffset (offset, size, 0.);
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
DefineReal (int size, const char *mnemonic)
{
  do
    {
      exprBuild ();
      ValueTag legal = ValueFloat | ValueSymbol | ValueCode;
      if (option_autocast)
	legal |= ValueInt;
      if (Reloc_QueueExprUpdate (DefineReal_RelocUpdater, areaCurrentSymbol->value.Data.Int.i,
				 legal, &size, sizeof (size)))
	error (ErrorError, "Illegal %s expression", mnemonic);

      skipblanks ();
    }
  while (Input_Match (',', false));
  return false;
}

/**
 * Implements DCFS (IEEE Single Precision).
 */
bool
c_dcfs (void)
{
  return DefineReal (4, "DCFS");
}

/**
 * Implements DCFD (IEEE Double Precision).
 */
bool
c_dcfd (void)
{
  return DefineReal (8, "DCFD");
}

static bool
symFlag (unsigned int flags, const char *err)
{
  const Lex lex = lexGetId ();
  if (lex.tag != LexId)
    {
      /* When the symbol is not known yet, it will automatically be created.  */
      Symbol *sym = symbolGet (&lex);
      if (Local_IsLocalLabel (sym->str))
        error (ErrorError, "Local labels cannot be %s", err);
      else
        sym->type |= flags;
    }
  return false;
}

/**
 * Implements EXPORT / GLOBL.
 */
bool
c_globl (void)
{
  return symFlag (SYMBOL_REFERENCE | SYMBOL_DECLARED, "exported");
}

/**
 * Implements STRONG.
 */
bool
c_strong (void)
{
  return symFlag (SYMBOL_STRONG, "marked 'strong'");
}

/**
 * Implements KEEP.
 */
bool
c_keep (void)
{
  return symFlag (SYMBOL_KEEP | SYMBOL_DECLARED, "marked 'keep'");
}

/**
 * Implements IMPORT.
 */
bool
c_import (void)
{
  Lex lex = lexGetId ();
  if (lex.tag != LexId)
    return false; /* Error is already given.  */

  Symbol *sym = symbolGet (&lex);
  sym->type |= SYMBOL_REFERENCE | SYMBOL_DECLARED;
  while (Input_Match (',', false))
    {
      Lex attribute = lexGetId ();
      if (!strncmp ("NOCASE", attribute.Data.Id.str, attribute.Data.Id.len))
	sym->type |= SYMBOL_NOCASE;
      else if (!strncmp ("WEAK", attribute.Data.Id.str, attribute.Data.Id.len))
	sym->type |= SYMBOL_WEAK;
      else if (!strncmp ("COMMON", attribute.Data.Id.str, attribute.Data.Id.len))
        {
	  skipblanks ();
	  if (Input_Match ('=', false))
	    error (ErrorError, "COMMON attribute needs size specification");
	  else
	    {
	      const Value *size = exprBuildAndEval (ValueInt);
	      switch (size->Tag)
	        {
		  case ValueInt:
		    Value_Assign (&sym->value, size);
		    sym->type |= SYMBOL_COMMON;
		    break;
		  default:
		    error (ErrorError, "Illegal COMMON attribute expression");
		    break;
	        }
	    }
	}
      else if (!strncmp ("FPREGARGS", attribute.Data.Id.str, attribute.Data.Id.len))
	sym->type |= SYMBOL_FPREGARGS;
      else
	error (ErrorError, "Illegal IMPORT attribute %s", attribute.Data.Id.str);
      skipblanks ();
    }
  return false;
}

/**
 * Called for GET / INCLUDE
 */
bool
c_get (void)
{
  char *filename;
  if ((filename = strdup (inputRest ())) == NULL)
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

  FS_PushFilePObject (filename);
  if (option_verbose)
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
  if ((filename = strdup (inputRest ())) == NULL)
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
  
  FS_PushFilePObject (filename);
  if (option_verbose)
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
  if ((idfn_text = strdup (inputRest ())) == NULL)
    errorOutOfMem();
  skiprest ();
  return false;
}

/**
 * Implements BIN.
 */
bool
c_bin (void)
{
  char *filename;
  if ((filename = strdup (inputRest ())) == NULL)
    errorOutOfMem ();
  char *cptr;
  for (cptr = filename; *cptr && !isspace ((unsigned char)*cptr); cptr++)
    /* */;
  *cptr = '\0';

  const char *newFilename;
  FILE *binfp = getInclude (filename, &newFilename);
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
  while (!feof (binfp))
    putData (1, getc (binfp));
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
    errorAbort ("Cannot use END within a macro");
  FS_PopPObject (false);
  return false;
}

/**
 * Implements ASSERT.
 */
bool
c_assert (void)
{
  const Value *value = exprBuildAndEval (ValueBool);
  if (value->Tag != ValueBool)
    error (ErrorError, "ASSERT expression must be boolean");
  else if (!value->Data.Bool.b)
    error (ErrorError, "Assertion failed");
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

  if (giveErr)
    error (ErrorError, "%.*s", (int)message->Data.String.len, message->Data.String.s);
  else
    printf ("%.*s\n", (int)message->Data.String.len, message->Data.String.s);
  return false;
}

/**
 * Implements OPT.
 */
bool
c_opt (void)
{
  inputRest();
  /* Do nothing.  This is for compatiblity with objasm.  */
  return false;
}

/**
 * Implements SUBT (subtitle) / TTL (title).
 */
bool
c_title (void)
{
  inputRest();
  /* Do nothing right now.  This command is for the benefit of error reporting */
  return false;
}
