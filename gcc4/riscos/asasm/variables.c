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
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
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
#include "output.h"
#include "os.h"
#include "put.h"
#include "symbol.h"
#include "value.h"
#include "variables.h"

static char
var_type (ValueTag type)
{
  switch (type)
    {
      case ValueInt:
	return 'A';
      case ValueBool:
	return 'L';
      case ValueString:
	return 'S';
      default:
	break;
    }
  return '?';
}


static void
assign_var (Symbol *sym, ValueTag type)
{
  sym->value.Tag.t = type;
  sym->value.Tag.v = type;
  switch (type)
    {
      case ValueInt:
	sym->value.ValueInt.i = 0;
	break;
      case ValueBool:
	sym->value.ValueBool.b = false;
	break;
      case ValueString:
	sym->value.ValueString.len = 0;
	/* We don't do malloc(0) as this can on some systems return NULL.  */
	if ((sym->value.ValueString.s = malloc (1)) == NULL)
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
static void
declare_var (const char *ptr, int len, ValueTag type, bool localMacro)
{
  Lex var = lexTempLabel (ptr, len);
  Symbol *sym = symbolFind (&var);
  if (sym != NULL)
    {
      if (sym->value.Tag.t != type)
	{
	  error (ErrorError, "'%.*s' is already declared as a %s",
	         len, ptr, valueTagAsString (sym->value.Tag.t));
	  inputRest ();
	  return;
	}
      error (ErrorWarning, "Redeclaration of %s variable '%.*s'",
	     valueTagAsString (sym->value.Tag.t),
	     var.LexId.len, var.LexId.str);
    }
  else
    sym = symbolAdd (&var);

  if (localMacro)
    sym->type |= SYMBOL_MACRO_LOCAL;
  assign_var (sym, type);
}


static const char *
var_inputSymbol (int *len)
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


void
c_gbl (ValueTag type, const Lex *label)
{
  if (label->tag != LexNone)
    error (ErrorWarning, "Label not allowed here - ignoring");
  skipblanks ();
  const char *ptr;
  int len;
  if (!inputComment ())
    ptr = var_inputSymbol (&len);
  else
    {
      len = 0;
      ptr = NULL;
    }
  if (!len)
    {
      error (ErrorError, "Missing variable name");
      return;
    }
  declare_var (ptr, len, type, false);
}


void
c_lcl (ValueTag type, const Lex *label)
{
  if (gCurPObjP->type != POType_eMacro)
    {
      error (ErrorError, "Local variables not allowed outside macros");
      inputRest ();
      return;
    }

  if (label->tag != LexNone)
    error (ErrorWarning, "Label not allowed here - ignoring");
  skipblanks ();
  const char *ptr;
  int len;
  if (!inputComment ())
    ptr = var_inputSymbol (&len);
  else
    {
      ptr = NULL;
      len = 0;
    }
  if (!len)
    {
      error (ErrorError, "Missing variable name");
      return;
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
}


void
c_set (ValueTag type, const Lex *label)
{
  const char *c;
  Symbol *sym = symbolFind (label);
  if (sym == NULL || sym->value.Tag.t == ValueIllegal)
    c = "undefined";
  else if (sym->value.Tag.v == ValueConst)
    c = "not a variable";
  else
    c = NULL;
  if (c)
    {
      error (ErrorError, "'%.*s' is %s", label->LexId.len, label->LexId.str, c);
      inputRest ();
      return;
    }
  exprBuild ();
  Value value = exprEval (sym->value.Tag.t);
  value.Tag.v = sym->value.Tag.v;
  sym->type |= SYMBOL_DEFINED;
  switch (value.Tag.t)
    {
      case ValueIllegal:
	error (ErrorError, "Illegal SET%c", var_type (type));
	sym->value.Tag.t = ValueInt;
	sym->value.ValueInt.i = 0;
	break;
#ifdef DEBUG
      case ValueString:
	printf ("c_set: string: <%.*s>\n", value.ValueString.len, value.ValueString.s);
	/* Fall through.  */
#endif
      default:
	sym->value = valueCopy (value);
	break;
    }
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
  int len = i ? i - def : (int)strlen (def);
  declare_var (def, len, ValueString, false);
  Lex var = lexTempLabel (def, len);
  if (!i)
    i = "";
  else
    i++;

  Value value;
  value.Tag.t = ValueString;
  value.Tag.v = ValueString;
  value.ValueString.len = strlen (i);
  value.ValueString.s = i;

  /* FIXME: symbolFind() can return NULL here, no ? */
  symbolFind (&var)->value = valueCopy (value);
}
