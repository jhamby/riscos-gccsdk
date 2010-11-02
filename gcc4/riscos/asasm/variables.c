/*
 * AS an assembler for ARM
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
 * variables.c
 */

#include "config.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "area.h"
#include "code.h"
#include "error.h"
#include "expr.h"
#include "filestack.h"
#include "fix.h"
#include "help_lex.h"
#include "include.h"
#include "input.h"
#include "lex.h"
#include "lit.h"
#include "macros.h"
#include "main.h"
#include "output.h"
#include "os.h"
#include "put.h"
#include "symbol.h"
#include "value.h"
#include "variables.h"

static void
assign_var (Symbol *sym, ValueTag type)
{
  sym->value.Tag = type;
  switch (type)
    {
      case ValueInt:
	sym->value.Data.Int.i = 0;
	break;
      case ValueBool:
	sym->value.Data.Bool.b = false;
	break;
      case ValueString:
	sym->value.Data.String.len = 0;
	/* We don't do malloc(0) as this can on some systems return NULL.  */
	if ((sym->value.Data.String.s = malloc (1)) == NULL)
	  errorOutOfMem ();
	break;
      default:
	error (ErrorAbort, "Internal error: assign_var");
	break;
    }
}


/**
 * (Re)define local or global variable.
 * When variable exists we'll give a warning when its type is the same,
 * when type is different an error is given instead.
 * \param localMacro Is true when variable is a local macro variable. Used to
 * implement :DEF:.
 */
static Symbol *
declare_var (const char *ptr, size_t len, ValueTag type, bool localMacro)
{
  Lex var = lexTempLabel (ptr, len);
  Symbol *sym = symbolFind (&var);
  if (sym != NULL)
    {
      if (sym->value.Tag != type)
	{
	  error (ErrorError, "'%.*s' is already declared as a %s",
	         (int)len, ptr, valueTagAsString (sym->value.Tag));
	  return NULL;
	}
      if (option_pedantic)
        error (ErrorWarning, "Redeclaration of %s variable '%.*s'",
	       valueTagAsString (sym->value.Tag),
	       (int)var.Data.Id.len, var.Data.Id.str);
    }
  else
    sym = symbolAdd (&var);

  if (localMacro)
    sym->type |= SYMBOL_MACRO_LOCAL;
  assign_var (sym, type);
  return sym;
}


static const char *
var_inputSymbol (size_t *len)
{
  char delim;
  if (inputLook () == '|')
    {
      inputSkip ();
      delim = '|';
    }
  else
    delim = 0;
  const char *sym = inputSymbol (len, delim);
  if (delim == '|')
    {
      if (inputLook () == '|')
	inputSkip ();
      else
	error (ErrorError, "Unterminated |label|");
    }
  return sym;
}


/**
 * Implements GBLL, GBLA and GBLS.
 * Global variable declaration
 */
bool
c_gbl (const Lex *label)
{
  int c = inputGet ();
  ValueTag type;
  switch (c)
    {
      case 'L':
	type = ValueBool;
        break;
      case 'A':
	type = ValueInt;
	break;
      case 'S':
	type = ValueString;
	break;
      default:
	return true;
    }

  if (label->tag != LexNone)
    error (ErrorWarning, "Label not allowed here - ignoring");

  skipblanks ();
  const char *ptr;
  size_t len;
  if (!Input_IsEolOrCommentStart ())
    ptr = var_inputSymbol (&len);
  else
    {
      len = 0;
      ptr = NULL;
    }
  if (!len)
    {
      error (ErrorError, "Missing variable name");
      return false;
    }
  declare_var (ptr, len, type, false);
  return false;
}


/**
 * Implements LCLL, LCLA and LCLS.
 * Local variable declaration
 */
bool
c_lcl (const Lex *label)
{
  int c = inputGet ();
  ValueTag type;
  switch (c)
    {
      case 'L':
	type = ValueBool;
        break;
      case 'A':
	type = ValueInt;
	break;
      case 'S':
	type = ValueString;
	break;
      default:
	return true;
    }

  if (gCurPObjP->type != POType_eMacro)
    {
      error (ErrorError, "Local variables not allowed outside macros");
      return false;
    }

  if (label->tag != LexNone)
    error (ErrorWarning, "Label not allowed here - ignoring");
  skipblanks ();
  const char *ptr;
  size_t len;
  if (!Input_IsEolOrCommentStart ())
    ptr = var_inputSymbol (&len);
  else
    {
      ptr = NULL;
      len = 0;
    }
  if (!len)
    {
      error (ErrorError, "Missing variable name");
      return false;
    }

  /* Link our local variable into the current macro so we can restore this
     at the end of macro invocation.  */
  Lex l = lexTempLabel (ptr, len);
  Symbol *sym = symbolFind (&l);
  varPos *p;
  if ((p = malloc (sizeof (varPos) + len + 1)) == NULL)
    errorOutOfMem ();
  memcpy (p->name, ptr, len + 1);
  p->next = gCurPObjP->d.macro.varListP;
  if ((p->symptr = sym) != NULL)
    p->symbol = *sym;
  gCurPObjP->d.macro.varListP = p;

  /* When symbol is already known, it remains a global variable (and :DEF:
     returns {TRUE} for it).  */
  declare_var (ptr, len, type, sym == NULL || (sym->type & SYMBOL_MACRO_LOCAL));
  return false;
}


/**
 * SETA, SETL, SETS implementation.
 * Variable assignment
 */
bool
c_set (const Lex *label)
{
  int c = inputGet ();
  ValueTag type;
  switch (c)
    {
      case 'L':
	type = ValueBool;
        break;
      case 'A':
	type = ValueInt;
	break;
      case 'S':
	type = ValueString;
	break;
      default:
	return true;
    }

  Symbol *sym = symbolFind (label);
  if (sym == NULL)
    {
      error (ErrorError, "'%.*s' is undefined",
	     (int)label->Data.Id.len, label->Data.Id.str);
      return false;
    }
  assert (sym->value.Tag != ValueIllegal);
  if (type != sym->value.Tag)
    {
      error (ErrorError, "Wrong type for symbol '%.*s'",
	     (int)label->Data.Id.len, label->Data.Id.str);
      return false;
    }
  Value value = exprBuildAndEval (sym->value.Tag);
  sym->type |= SYMBOL_DEFINED;
  switch (value.Tag)
    {
      case ValueIllegal:
	error (ErrorError, "Illegal SET%c", c);
	sym->value.Tag = ValueInt;
	sym->value.Data.Int.i = 0;
	break;
#ifdef DEBUG
      case ValueString:
	printf ("c_set: string: <%.*s>\n",
		(int)value.Data.String.len, value.Data.String.s);
	/* Fall through.  */
#endif
      default:
	sym->value = valueCopy (value);
	break;
    }
  return false;
}


/**
 * Called at the end of each MACRO execution to restore all its local
 * variables.
 */
void
var_restoreLocals (const varPos *p)
{
  while (p != NULL)
    {
      if (p->symptr)
	{
	  /* Variable existed before we (temporarily) overruled it, so
	     restore it to its original value.  */
	  valueFree (&p->symptr->value);
	  assert (p->symptr->next == p->symbol.next);
	  *p->symptr = p->symbol;
	}
      else
	{
	  Lex l = lexTempLabel (p->name, strlen (p->name));
	  symbolRemove (&l);
	}

      const varPos *q = p->next;
      free ((void *)p);
      p = q;
    }
}


void
var_define (const char *def)
{
  const char *i = strchr (def, '=');
  size_t len = i != NULL ? (size_t)(i - def) : strlen (def);
  Symbol *sym = declare_var (def, len, ValueString, false);
  if (sym == NULL)
    return;

  size_t datLen;
  if (i == NULL)
    {
      i = "";
      datLen = 0;
    }
  else
    {
      ++i; /* Skip '=' */
      datLen = strlen (i);
    }
  const Value value =
    {
      .Tag = ValueString,
      .Data.String = { .len = datLen, .s = i }
    };
  sym->value = valueCopy (value);
}
