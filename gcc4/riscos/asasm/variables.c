/*
 * AS an assembler for ARM
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
#include "include.h"
#include "input.h"
#include "lex.h"
#include "lit.h"
#include "macros.h"
#include "main.h"
#include "output.h"
#include "put.h"
#include "symbol.h"
#include "value.h"
#include "variables.h"

#ifdef DEBUG
//#  define DEBUG_VARIABLES
#endif

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
 * When variable exists we'll give a pedantic warning when its type is the same,
 * when type is different an error is given instead.
 * \param localMacro Is true when variable is a local macro variable. Used to
 * implement :DEF:.
 */
static Symbol *
declare_var (const char *ptr, size_t len, ValueTag type, bool localMacro)
{
  const Lex var = lexTempLabel (ptr, len);
  Symbol *sym = symbolFind (&var);
  if (sym != NULL)
    {
      if (sym->type & SYMBOL_AREA)
	{
	  error (ErrorError, "'%.*s' is already defined as an area",
		 (int)len, ptr);
	  return NULL;
	}
      /* A local variable can have a different type than a similar named
	 global variable.  */
      /* FIXME: this is not 100% bullet proof: a local variable with same
	 name as global one will not get SYMBOL_MACRO_LOCAL bit so remains
	 local.  So we won't detect two different type local variables (with
	 same name) when we have a global variable also with same name).  */
      if (sym->value.Tag != type
          && (!localMacro || (sym->type & SYMBOL_MACRO_LOCAL) != 0))
	{
	  if (sym->value.Tag == ValueIllegal)
	    error (ErrorError, "'%.*s' is already %s",
		   (int)len, ptr,
		   sym->type & SYMBOL_DEFINED ? "defined" : "declared");
	  else
	    error (ErrorError, "'%.*s' is already %s as a %s",
		   (int)len, ptr,
		   sym->type & SYMBOL_DEFINED ? "defined" : "declared",
		   valueTagAsString (sym->value.Tag));
	  return NULL;
	}
      if (option_pedantic)
	error (ErrorWarning, "Redeclaration of %s variable '%.*s'",
	       valueTagAsString (sym->value.Tag),
	       (int)var.Data.Id.len, var.Data.Id.str);

      /* When symbol is already known as global, it remains a global variable
	 (and :DEF: returns {TRUE} for it).  */
      localMacro = false;
    }
  else
    sym = symbolGet (&var);

  sym->type |= SYMBOL_DEFINED | SYMBOL_ABSOLUTE | SYMBOL_RW;
  if (localMacro)
    sym->type |= SYMBOL_MACRO_LOCAL;
  assign_var (sym, type);
  return sym;
}


/**
 * Implements GBLL, GBLA and GBLS.
 * Global variable declaration
 */
bool
c_gbl (void)
{
  ValueTag type;
  const char c  = inputLook ();
  if (c == 'L')
    type = ValueBool;
  else if (c == 'A')
    type = ValueInt;
  else if (c == 'S')
    type = ValueString;
  else
    return true;
  inputSkip ();
  if (!Input_IsEndOfKeyword ())
    return true;

  skipblanks ();
  const char *ptr;
  size_t len;
  if ((ptr = Input_Symbol (&len)) == NULL)
    error (ErrorError, "Missing variable name");
  else
    declare_var (ptr, len, type, false);
  return false;
}


/**
 * Implements LCLL, LCLA and LCLS.
 * Local variable declaration
 */
bool
c_lcl (void)
{
  ValueTag type;
  const char c  = inputLook ();
  if (c == 'L')
    type = ValueBool;
  else if (c == 'A')
    type = ValueInt;
  else if (c == 'S')
    type = ValueString;
  else
    return true;
  inputSkip ();
  if (!Input_IsEndOfKeyword ())
    return true;

  if (gCurPObjP->type != POType_eMacro)
    {
      error (ErrorError, "Local variables not allowed outside macros");
      return false;
    }

  skipblanks ();
  const char *ptr;
  size_t len;
  if ((ptr = Input_Symbol (&len)) == NULL)
    {
      error (ErrorError, "Missing variable name");
      return false;
    }

  /* Link our local variable into the current macro so we can restore this
     at the end of macro invocation.  */
  const Lex l = lexTempLabel (ptr, len);
  Symbol *symbolP = symbolFind (&l);

  bool doRestore;
  if (symbolP != NULL)
    {
      /* Perhaps already made local ? */
      VarPos *varPosP;
      for (varPosP = gCurPObjP->d.macro.varListP;
	   varPosP != NULL && varPosP->symbolP != symbolP;
	   varPosP = varPosP->next)
	/* */;
      doRestore = varPosP == NULL;
    }
  else
    doRestore = true;

  if (doRestore)
    {
      VarPos *p;
      if ((p = malloc (sizeof (VarPos) + len + 1)) == NULL)
	errorOutOfMem ();
      memcpy (p->name, ptr, len); p->name[len] = '\0';
      p->next = gCurPObjP->d.macro.varListP;
      if ((p->symbolP = symbolP) != NULL)
	{
	  p->symbol = *symbolP;
	  /* Reset Symbol parts which won't get touched by declare_var().  */
	  symbolP->codeSize = 0;
	  symbolP->areaDef = areaCurrentSymbol;
	}
      gCurPObjP->d.macro.varListP = p;
    }
  
  declare_var (ptr, len, type, true);
  return false;
}


/**
 * SETA, SETL, SETS implementation.
 * Variable assignment
 */
bool
c_set (const Lex *label)
{
  ValueTag type;
  const char c  = inputLook ();
  if (c == 'L')
    type = ValueBool;
  else if (c == 'A')
    type = ValueInt;
  else if (c == 'S')
    type = ValueString;
  else
    return true;
  inputSkip ();
  if (!Input_IsEndOfKeyword ())
    return true;

  Symbol *sym = symbolFind (label);
  if (sym == NULL)
    {
      error (ErrorError, "'%.*s' is undefined",
	     (int)label->Data.Id.len, label->Data.Id.str);
      return false;
    }
  if (type != sym->value.Tag)
    {
      error (ErrorError, "Wrong type for symbol '%.*s'",
	     (int)label->Data.Id.len, label->Data.Id.str);
      return false;
    }
  const Value *value = exprBuildAndEval (sym->value.Tag);
  switch (value->Tag)
    {
      case ValueIllegal:
	error (ErrorError, "Wrong variable type for '%.*s'", (int)label->Data.Id.len, label->Data.Id.str);
	break;

      default:
	sym->type |= SYMBOL_DEFINED;
	Value_Assign (&sym->value, value);
	break;
    }

  return false;
}


/**
 * Called at the end of each MACRO execution to restore all its local
 * variables.
 */
void
Var_RestoreLocals (const VarPos *p)
{
  while (p != NULL)
    {
      if (p->symbolP)
	{
	  /* Variable existed before we (temporarily) overruled it, so
	     restore it to its original value.  */
	  valueFree (&p->symbolP->value);
	  *p->symbolP = p->symbol;
	}
      else
	{
	  const Lex l = lexTempLabel (p->name, strlen (p->name));
	  symbolRemove (&l);
	}

      const VarPos *q = p->next;
      free ((void *)p);
      p = q;
    }
}


void
Var_Define (const char *def)
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
  Value_Assign (&sym->value, &value);
}
