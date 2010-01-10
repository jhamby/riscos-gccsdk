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

typedef struct varPos
  {
    struct varPos *next;
    char *name;
    Symbol *symptr;
    Symbol symbol;
  }
varPos;


static varPos *varList = NULL;


static char
var_type (ValueTag type)
{
  switch (type)
    {
    case ValueInt:
      return 'I';
    case ValueBool:
      return 'L';
    case ValueString:
      return 'S';
    default:
      return '?';
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
      sym->value.ValueBool.b = FALSE;
      break;
    case ValueString:
      sym->value.ValueString.len = 0;
      if ((sym->value.ValueString.s = calloc (1, 1)) == NULL)
        errorOutOfMem("assign_var");
      break;
    default:
      abort ();
      break;
    }
}


/**
 * (Re)define local or global variable.
 * When variable exists we'll give a warning when its type is the same,
 * when type is different an error is given.
 * Contents pointed by ptr has to remain valid all the time.
 */
static void
declare_var (const char *ptr, int len, ValueTag type)
{
  Lex var = lexTempLabel (ptr, len);
  Symbol *sym = symbolFind (&var);
  if (sym != NULL)
    {
      if (sym->value.Tag.t != type)
	{
	  error (ErrorError, TRUE, "'%.*s' is already declared as a %s",
	         len, ptr, valueTagAsString (sym->value.Tag.t));
	  inputRest ();
	  return;
	}
      error (ErrorWarning, TRUE, "Redeclaration of %s variable '%.*s'",
	     valueTagAsString (sym->value.Tag.t),
	     var.LexId.len, var.LexId.str);
    }
  else
    {
      sym = symbolAdd (&var);
      if (!sym)
	return;
    }
  assign_var (sym, type);
}


static const char *
var_inputSymbol (int *len)
{
  char delim = '|', *sym;
  if (inputLook () == '|')
    inputSkip ();
  else
    delim = 0;
  sym = inputSymbol (len, delim);
  if (delim == '|')
    {
      if (inputLook () == '|')
	inputSkip ();
      else
	error (ErrorError, TRUE, "Unterminated |label|");
    }
  return sym;
}


void
c_gbl (ValueTag type, Lex * label)
{
  if (label->tag != LexNone)
    error (ErrorWarning, TRUE, "Label not allowed here - ignoring");
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
      error (ErrorError, TRUE, "Missing variable name");
      return;
    }
  declare_var (ptr, len, type);
}


void
c_lcl (ValueTag type, Lex *label)
{
  if (!macroSP)
    {
      error (ErrorError, TRUE, "Local variables not allowed outside macros");
      inputRest ();
      return;
    }
  if (label->tag != LexNone)
    error (ErrorWarning, TRUE, "Label not allowed here - ignoring");
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
      error (ErrorError, TRUE, "Missing variable name");
      return;
    }
  Lex l = lexTempLabel (ptr, len);
  Symbol *sym = symbolFind (&l);
  varPos *p;
  if ((p = malloc (sizeof (varPos))) == NULL)
    goto nomem;
  p->next = varList;
  if ((p->name = strndup (ptr, len)) == NULL)
    goto nomem;
  p->symptr = sym;
  declare_var (ptr, len, type);
  varList = p;
  return;

nomem:
  errorOutOfMem ("c_lcl");
}


void
c_set (ValueTag type, Lex * label)
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
      error (ErrorError, TRUE, "'%.*s' is %s",
             label->LexId.len, label->LexId.str, c);
      inputRest ();
      return;
    }
  exprBuild ();
  Value value = exprEval (sym->value.Tag.t);
  value.Tag.v = sym->value.Tag.v;
  sym->type = SYMBOL_DEFINED;
  switch (value.Tag.t)
    {
    case ValueIllegal:
      error (ErrorError, TRUE, "Illegal SET%c", var_type (type));
      sym->value.Tag.t = ValueInt;
      sym->value.ValueInt.i = 0;
      break;
#ifdef DEBUG
    case ValueString:
      printf ("c_set: string: <%.*s>\n", value.ValueString.len, value.ValueString.s);
#endif
    default:
      sym->value = valueCopy (value);
      break;
    }
}


void
var_restoreLocals (void)
{
  varPos *p, *q = varList;

  while (p = q, p)
    {
      if (p->symptr)
	{
	  if (p->symptr->value.Tag.t == ValueString)
	    free ((void *)p->symptr->value.ValueString.s);
	  memcpy (p->symptr, &p->symbol, sizeof (Symbol));
	}
      else
	{
	  Lex l = lexTempLabel (p->name, strlen (p->name));
	  Symbol *sym;
	  if ((sym = symbolFind (&l)) != NULL)
	    {
	      sym->type = 0;	/* undefined */
	      sym->value.Tag.t = ValueIllegal;
	    }
	}
      q = p->next;
      free (p->name);
      free (p);
    }
  varList = NULL;
}


/* Contents pointed by def has to remain valid all the time.
 */
void
var_define (const char *def)
{
  Lex var;
  Value value;
  int len;

  const char *i = strchr (def, '=');
  len = i ? i - def : (int)strlen (def);
  declare_var (def, len, ValueString);
  var = lexTempLabel (def, len);
  if (!i)
    i = "";
  else
    i++;
  value.Tag.t = ValueString;
  value.Tag.v = ValueString;
  value.ValueString.len = strlen (i);
  value.ValueString.s = i;
  /* FIXME: symbolFind() can return NULL here, no ? */
  symbolFind (&var)->value = valueCopy (value);
}
