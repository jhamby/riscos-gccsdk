/*
 * commands.c
 * Copyright © 1992 Niklas Röjemo, 1997 Darren Salt
 */

#include "sdk-config.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "commands.h"
#include "error.h"
#include "input.h"
#include "output.h"
#include "lex.h"
#include "help_lex.h"
#include "expr.h"
#include "code.h"
#include "value.h"
#include "put.h"
#include "fix.h"
#include "include.h"
#include "filestack.h"
#include "area.h"
#include "lit.h"
#include "macros.h"
#include "strdup.h"
#include "hash.h"
#include "symbol.h"
#include "local.h"
#include "os.h"

extern FILE *asmfile;		/* in input.c */


static Symbol *
c_define (char *msg, Symbol * sym, ValueTag legal)
{
  Value value;
  if (!sym)
    error (ErrorError, FALSE, "Missing label before %s", msg);
  sym->type |= SYMBOL_ABSOLUTE;
  exprBuild ();
  value = exprEval (legal);
  if (value.Tag.t == ValueIllegal)
    {
      error (ErrorError, TRUE, "Illegal %s", msg);
      sym->value.Tag.t = ValueInt;
      sym->value.ValueInt.i = 0;
    }
  else
    sym->value = valueCopy (value);
  sym->type |= SYMBOL_DEFINED;
  sym->declared = 1;
  sym->area.ptr = 0;
  return sym;
}


void 
c_equ (Symbol * symbol)
{
  (void) c_define ("equ", symbol, ValueAll);
}


void 
c_fn (Symbol * symbol)
{
  int no = c_define ("float register", symbol, ValueInt)->value.ValueInt.i;
  symbol->type |= SYMBOL_FPUREG;
  if (no < 0 || no > 7)
    {
      symbol->value.ValueInt.i = 0;
      error (ErrorError, TRUE, "Illegal %s register %d (using 0)", "fpu", no);
    }
}


void 
c_rn (Symbol * symbol)
{
  int no = c_define ("register", symbol, ValueInt)->value.ValueInt.i;
  symbol->type |= SYMBOL_CPUREG;
  if (no < 0 || no > 15)
    {
      symbol->value.ValueInt.i = 0;
      error (ErrorError, TRUE, "Illegal %s register %d (using 0)", "cpu", no);
    }
}


void 
c_cn (Symbol * symbol)
{
  int no = c_define ("coprocessor register", symbol, ValueInt)->value.ValueInt.i;
  symbol->type |= SYMBOL_COPREG;
  if (no < 0 || no > 15)
    {
      symbol->value.ValueInt.i = 0;
      error (ErrorError, TRUE, "Illegal %s register %d (using 0)", "cop", no);
    }
}

void 
c_cp (Symbol * symbol)
{
  int no = c_define ("coprocessor number", symbol, ValueInt)->value.ValueInt.i;
  symbol->type |= SYMBOL_COPNUM;
  if (no < 0 || no > 15)
    {
      symbol->value.ValueInt.i = 0;
      error (ErrorError, TRUE, "Illegal coprocessor number %d (using 0)", no);
    }
}


void 
c_ltorg (void)
{
  if (areaCurrent)
    litOrg (areaCurrent->area.info->lits);
  else
    areaError ();
}


static void 
defineint (int size)
{
  Value value;
  WORD word = 0;
  int c;
  do
    {
      skipblanks ();
      exprBuild ();
      value = exprEval (ValueInt | ValueString | ValueCode | ValueLateLabel | ValueAddr);
      switch (value.Tag.t)
	{
	case ValueInt:
	case ValueAddr:
	  word = fixInt (inputLineNo, size, value.ValueInt.i);
	  putData (size, word);
	  break;
	case ValueString:
	  if (size == 1)
	    {			/* Lay out a string */
	      int len = value.ValueString.len;
	      char *str = value.ValueString.s;
	      while (len > 0)
		putData (1, lexGetCharFromString (&len, &str));
	    }
	  else
	    putData (size, lexChar2Int (FALSE, value.ValueString.len, value.ValueString.s));
	  break;
	case ValueCode:
	case ValueLateLabel:
	  relocInt (size, value);
	  putData (size, word);
	  break;
	default:
	  error (ErrorError, TRUE, "Illegal %s expression", "int");
	}
      skipblanks ();
    }
  while (((c = inputGet ()) != 0) && c == ',');
  inputUnGet (c);
}


void 
c_head (void)
{
  int i;
  Value value;

  i = areaCurrent ? areaCurrent->value.ValueInt.i : 0;
  skipblanks ();
  exprBuild ();
  value = exprEval (ValueString);
  switch (value.Tag.t)
    {
    case ValueString:
      if (areaCurrent)
	{
	  int len = value.ValueString.len;
	  char *str = value.ValueString.s;
	  while (len > 0)
	    putData (1, lexGetCharFromString (&len, &str));
	  putData (1, 0);
	}
      break;
    default:
      error (ErrorError, TRUE, "Illegal %s expression", "string");
    }
  if (areaCurrent)
    {
      while (areaCurrent->value.ValueInt.i & 3)
	areaCurrent->area.info->image[areaCurrent->value.ValueInt.i++] = 0;
      putData (4, 0xFF000000 + (areaCurrent->value.ValueInt.i - i));
    }
  else
    areaError ();
}


void 
c_dcb (void)
{
  defineint (1);
}


void 
c_dcw (void)
{
  defineint (2);
}


void 
c_dcd (void)
{
  defineint (4);
}

static void 
definereal (int size)
{
  Value value;
  int c;
  do
    {
      skipblanks ();
      exprBuild ();
      value = exprEval (ValueInt | ValueFloat | ValueLateLabel | ValueCode);
      switch (value.Tag.t)
	{
	case ValueInt:
	  putDataFloat (size, (FLOAT) value.ValueInt.i);
	  break;
	case ValueFloat:
	  putDataFloat (size, value.ValueFloat.f);
	  break;
	case ValueCode:
	case ValueLateLabel:
	  relocFloat (size, value);
	  putDataFloat (size, 0.0);
	  break;
	default:
	  error (ErrorError, TRUE, "Illegal %s expression", "float");
	}
      skipblanks ();
    }
  while (((c = inputGet ()) != 0) && c == ',');
  inputUnGet (c);
}


void 
c_dcfd (void)
{
  definereal (8);
}


void 
c_dcfe (void)
{
  error (ErrorError, FALSE, "Not implemented: dcf%c %s", 'e', inputRest ());
}


void 
c_dcfs (void)
{
  definereal (4);
}


void 
c_dcfp (void)
{
  error (ErrorError, FALSE, "Not implemented: dcf%c %s", 'p', inputRest ());
}


static void 
symFlag (int flag, int declared, const char *err)
{
  Symbol *sym;
  Lex lex = lexGetId ();
  if (lex.tag != LexId)
    return;
  sym = symbolGet (lex);
  if (localTest (sym->str))
    error (ErrorError, TRUE, "Local labels cannot be %s", err);
  else
    {
      sym->type |= flag;
      if (declared)
	sym->declared = 1;
    }
}


void 
c_globl (void)
{
  symFlag (SYMBOL_REFERENCE, 1, "exported");
}


void 
c_strong (void)
{
  symFlag (SYMBOL_STRONG, 0, "marked 'strong'");
}


void 
c_keep (void)
{
  symFlag (SYMBOL_KEEP, 1, "marked 'keep'");
}


void 
c_import (void)
{
  int c;
  Symbol *sym;
  Lex lex = lexGetId ();
  if (lex.tag != LexId)
    return;
  sym = symbolGet (lex);
  sym->type |= SYMBOL_REFERENCE;
  sym->declared = 1;
  if ((c = inputGet ()) == ',')
    {
      Lex attribute = lexGetId ();
      if (!strncmp ("WEAK", attribute.LexId.str, attribute.LexId.len))
	sym->type |= SYMBOL_WEAK;
      else
	error (ErrorError, TRUE, "Illegal IMPORT attribute %s", attribute.LexId.str);
    }
}


extern int verbose;


void 
c_get (void)
{
  extern int gcc_backend;
  char *filename, *cptr;
  FILE *getfp;

  if (macroSP)
    {
      error (ErrorSerious, TRUE, "Cannot use GET within a macro");
      skiprest ();
      return;
    }
  inputExpand = FALSE;
  filename = strdup (inputRest ());
  if (!filename)
    errorOutOfMem ("c_get");
  for (cptr = filename; *cptr && !isspace (*cptr); cptr++);
  if (*cptr)
    {
      *cptr++ = '\0';		/* must be a space */
      while (*cptr && isspace (*cptr))
	cptr++;
      if (*cptr && *cptr != ';' && (!gcc_backend || *cptr != '@'))
	{
	  error (ErrorError, FALSE, "Skipping extra characters '%s' after filename", cptr);
	}
    }
  getfp = getInclude (filename, "r");
  if (!getfp)
    {
      error (ErrorError, TRUE, "Cannot open file \"%s\"", filename);
      return;
    }
  push_file (asmfile);
  inputLineNo = 0;
#ifdef __riscos__
  dependPut (" ", filename, "");
  inputName = CanonicalisePath (filename);
#endif
  asmfile = getfp;
  if (verbose)
    fprintf (stderr, "Including file %s\n", filename);
}


void 
c_lnk (void)
{
  char *filename, *cptr;
  FILE *lnkfp;

  if (macroSP)
    {
      error (ErrorSerious, TRUE, "Cannot use LNK within a macro");
      skiprest ();
      return;
    }
  inputExpand = FALSE;
  if (if_depth)
    if_depth--;
  testUnmatched ();
  filename = strdup (inputRest ());
  if (!filename)
    errorOutOfMem ("c_lnk");
#ifdef __riscos
  dependPut (" ", filename, "");
#endif
  for (cptr = filename; *cptr && !isspace (*cptr); cptr++);
  *cptr = '\0';
  inputNextLine ();
  lnkfp = getInclude (filename, "r");
  if (!lnkfp)
    {
      error (ErrorError, TRUE, "Cannot open file \"%s\"", filename);
      return;
    }
  skiprest ();
  inputFinish ();
  inputLineNo = 0;
#ifdef __riscos
  inputName = CanonicalisePath (filename);
#else
  inputName = filename;
#endif
  if_depth = 0;
  asmfile = lnkfp;
  if (verbose)
    fprintf (stderr, "Linking to file %s\n", filename);
}


void 
c_idfn (void)
{
  idfn_text = strdup (inputRest ());
  skiprest ();
}


void 
c_bin (void)
{
  char *filename, *cptr;
  FILE *binfp;

  inputExpand = FALSE;
  filename = strdup (inputRest ());
  if (!filename)
    errorOutOfMem ("c_bin");
  for (cptr = filename; *cptr && !isspace (*cptr); cptr++);
  *cptr = '\0';

  binfp = getInclude (filename, "r");
  if (!binfp)
    {
      error (ErrorError, TRUE, "Cannot open file \"%s\"", filename);
      return;
    }
  if (verbose)
    fprintf (stderr, "Including binary file %s\n", filename);
  while (!feof (binfp))
    putData (1, getc (binfp));
  fclose (binfp);
}


extern int returnvalue;


void 
c_end (void)
{
  FILE *fp;

  if (macroCurrent)
    {
      error (ErrorSerious, TRUE, "Cannot use END within a macro");
      return;
    }
  fp = pop_file ();
  if (!fp)
    returnvalue = 1;
  else
    {
      fclose (asmfile);
      asmfile = fp;
      if (verbose)
	fprintf (stderr, "Returning from include file\n");
    }
}


void 
c_assert (void)
{
  Value value;

  exprBuild ();
  value = exprEval (ValueBool);
  if (value.Tag.t != ValueBool)
    error (ErrorError, TRUE, "ASSERT expression must be boolean");
  else if (!value.ValueBool.b)
    error (ErrorError, TRUE, "Assertion failed");
}
