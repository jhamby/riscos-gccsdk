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
#include "commands.h"
#include "error.h"
#include "expr.h"
#include "filestack.h"
#include "fix.h"
#include "help_lex.h"
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
  Value value = exprBuildAndEval (legal);
  if (value.Tag == ValueIllegal)
    {
      error (ErrorError, "Illegal %s", msg);
      sym->value.Tag = ValueInt;
      sym->value.Data.Int.i = 0;
    }
  else
    sym->value = valueCopy (value);
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
  litOrg (areaCurrentSymbol->area.info->lits);
  return false;
}

static void
defineint (int size)
{
  int c;
  do
    {
      ARMWord word = 0;
      skipblanks ();
      Value value = exprBuildAndEval (ValueInt | ValueString | ValueCode | ValueLateLabel | ValueAddr);
      switch (value.Tag)
	{
	case ValueInt:
	case ValueAddr:
	  word = fixInt (0, size, value.Data.Int.i);
	  putData (size, word);
	  break;
	case ValueString:
	  if (size == 1)
	    { /* Lay out a string.  */
	      size_t len = value.Data.String.len;
	      const char *str = value.Data.String.s;
	      for (size_t i = 0; i < len; ++i)
		putData (1, str[i]);
	    }
	  else
	    putData (size, lexChar2Int (false, value.Data.String.len, value.Data.String.s));
	  break;
	case ValueCode:
	case ValueLateLabel:
	  relocInt (size, &value);
	  putData (size, word);
	  break;
	default:
	  error (ErrorError, "Illegal %s expression", "int");
	  break;
	}
      skipblanks ();
    }
  while (((c = inputGet ()) != 0) && c == ',');
  inputUnGet (c);
}

/**
 * Implements "HEAD" : APCS function name signature.
 * ObjAsm extension.
 */
bool
c_head (void)
{
  skipblanks ();
  Value value = exprBuildAndEval (ValueString);
  switch (value.Tag)
    {
      case ValueString:
	{
	  size_t len = value.Data.String.len;
	  const char *str = value.Data.String.s;
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
 * Implements DCB and = (8 bit integer).
 */
bool
c_dcb (void)
{
  defineint (1);
  return false;
}

/**
 * Implements DCW (16 bit integer).
 */
bool
c_dcw (void)
{
  defineint (2);
  return false;
}

/**
 * Implements DCD and & (32 bit integer).
 */
bool
c_dcd (void)
{
  defineint (4);
  return false;
}

static void
definereal (int size)
{
  int c;
  do
    {
      skipblanks ();
      Value value = exprBuildAndEval (ValueInt | ValueFloat | ValueLateLabel | ValueCode);
      switch (value.Tag)
	{
	case ValueInt:
	  putDataFloat (size, value.Data.Int.i);
	  break;
	case ValueFloat:
	  putDataFloat (size, value.Data.Float.f);
	  break;
	case ValueCode:
	case ValueLateLabel:
	  relocFloat (size, &value);
	  putDataFloat (size, 0.0);
	  break;
	default:
	  error (ErrorError, "Illegal %s expression", "float");
	  break;
	}
      skipblanks ();
    }
  while (((c = inputGet ()) != 0) && c == ',');
  inputUnGet (c);
}

/**
 * Implements DCFS (IEEE Single Precision).
 */
bool
c_dcfs (void)
{
  definereal (4);
  return false;
}

/**
 * Implements DCFD (IEEE Double Precision).
 */
bool
c_dcfd (void)
{
  definereal (8);
  return false;
}

static bool
symFlag (unsigned int flags, const char *err)
{
  const Lex lex = lexGetId ();
  if (lex.tag != LexId)
    {
      /* When the symbol is not known yet, it will automatically be created.  */
      Symbol *sym = symbolGet (&lex);
      if (localTest (sym->str))
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
	      Value size = exprBuildAndEval (ValueInt);
	      switch (size.Tag)
	        {
		  case ValueInt:
		    sym->value = valueCopy (size);
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
  Value value = exprBuildAndEval (ValueBool);
  if (value.Tag != ValueBool)
    error (ErrorError, "ASSERT expression must be boolean");
  else if (!value.Data.Bool.b)
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
  Value value = exprBuildAndEval (ValueInt | ValueFloat);

  skipblanks();
  if (!Input_Match (',', false))
    {
      error (ErrorError, "Missing , in INFO directive");
      return false;
    }

  Value message = exprBuildAndEval (ValueString);
  if (message.Tag != ValueString)
    {
      error (ErrorError, "INFO message must be a string");
      return false;
    }

  if (value.Tag != ValueInt && value.Tag != ValueFloat)
    error (ErrorError, "INFO expression must be arithmetic");
  else
    {
      bool giveErr = (value.Tag == ValueInt && value.Data.Int.i != 0)
	               || (value.Tag == ValueFloat && fabs (value.Data.Float.f) >= 0.00001);
      if (giveErr)
        error (ErrorError, "%.*s", (int)message.Data.String.len, message.Data.String.s);
      else
	printf ("%.*s\n", (int)message.Data.String.len, message.Data.String.s);
    }
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
