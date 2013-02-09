/*
 * AS an assembler for ARM
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2000-2013 GCCSDK Developers
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

static void Var_PredefineVariables (void);
static Symbol *Var_Define (const char *ptr, size_t len, ValueTag type, bool localMacro);

/**
 * Variables phase preparation.
 */
void
Var_PrepareForPhase (Phase_e phase)
{
  switch (phase)
    {
      case ePassOne:
	Var_PredefineVariables ();
	break;

      case ePassTwo:
	/* Delete all variables so that a :DEF: test returns the same
	   result as in ePassOne.  */
	Symbol_RemoveVariables ();
	Var_PredefineVariables ();
	break;

      default:
	break;
    }
}


static void
Var_PredefineVariables (void)
{
  Symbol *sym = Var_Define ("asasm$version", sizeof ("asasm$version")-1,
			    ValueInt, false);
  if (sym != NULL)
    sym->value.Data.Int.i = ASASM_VERSION;
}

/**
 * (Re)define local or global variable.
 * When variable exists we'll give a pedantic warning when its type is the same,
 * when type is different an error is given instead.
 * \param localMacro Is true when variable is a local macro variable. Used to
 * implement :DEF:.
 */
static Symbol *
Var_Define (const char *ptr, size_t len, ValueTag type, bool localMacro)
{
  const Lex var = Lex_Id (ptr, len);
  Symbol *sym = Symbol_Get (&var);

  if (sym->type & (SYMBOL_REFERENCE | SYMBOL_STRONG | SYMBOL_KEEP))
    {
      Error (ErrorError, "'%.*s' is already in use as symbol and can not be used as %s variable",
	     (int)len, ptr, localMacro ? "local" : "global");
      return NULL;
    }
  
  if (sym->value.Tag != ValueIllegal)
    {
      if (sym->type & SYMBOL_AREA)
	{
	  Error (ErrorError, "'%.*s' is already defined as an area",
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
	    Error (ErrorError, "'%.*s' is already defined", (int)len, ptr);
	  else
	    Error (ErrorError, "'%.*s' is already defined as a %s",
		   (int)len, ptr, Value_TagAsString (sym->value.Tag));
	  return NULL;
	}
      if (option_pedantic)
	Error (ErrorWarning, "Redefinition of %s variable '%.*s'",
	       Value_TagAsString (sym->value.Tag),
	       (int)var.Data.Id.len, var.Data.Id.str);

      /* When symbol is already known as global, it remains a global variable
	 (and :DEF: returns {TRUE} for it).  */
      localMacro = false;
    }

  sym->type |= SYMBOL_DEFINED | SYMBOL_ABSOLUTE | SYMBOL_RW;
  if (localMacro)
    sym->type |= SYMBOL_MACRO_LOCAL;

  switch (type)
    {
      case ValueInt:
	sym->value = Value_Int (0, eIntType_PureInt);
	break;

      case ValueBool:
	sym->value = Value_Bool (false);
	break;

      case ValueString:
	sym->value.Tag = type;
	sym->value.Data.String.len = 0;
	/* We don't do malloc(0) as this can on some systems return NULL.  */
	if ((sym->value.Data.String.s = malloc (1)) == NULL)
	  Error_OutOfMem ();
	break;

      default:
	assert (0);
	break;
    }

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
  const char c  = Input_Look ();
  if (c == 'L')
    type = ValueBool;
  else if (c == 'A')
    type = ValueInt;
  else if (c == 'S')
    type = ValueString;
  else
    return true;
  Input_Skip ();
  if (!Input_IsEndOfKeyword ())
    return true;

  Input_SkipWS ();
  const char *ptr;
  size_t len;
  if ((ptr = Input_Symbol (&len)) == NULL)
    Error (ErrorError, "Missing variable name");
  else
    Var_Define (ptr, len, type, false);
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
  const char c  = Input_Look ();
  if (c == 'L')
    type = ValueBool;
  else if (c == 'A')
    type = ValueInt;
  else if (c == 'S')
    type = ValueString;
  else
    return true;
  Input_Skip ();
  if (!Input_IsEndOfKeyword ())
    return true;

  if (gCurPObjP->type != POType_eMacro)
    {
      Error (ErrorError, "Local variables not allowed outside macros");
      return false;
    }

  Input_SkipWS ();
  const char *ptr;
  size_t len;
  if ((ptr = Input_Symbol (&len)) == NULL)
    {
      Error (ErrorError, "Missing variable name");
      return false;
    }

  /* Link our local variable into the current macro so we can restore this
     at the end of macro invocation.  */
  const Lex l = Lex_Id (ptr, len);
  Symbol *symbolP = Symbol_Find (&l);

  bool doRestore;
  if (symbolP != NULL)
    {
      /* Perhaps already made local ? */
      const VarPos *varPosP;
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
	Error_OutOfMem ();
      memcpy (p->name, ptr, len); p->name[len] = '\0';
      p->next = gCurPObjP->d.macro.varListP;
      if ((p->symbolP = symbolP) != NULL)
	{
	  p->symbol = *symbolP;
	  /* Reset Symbol parts which won't get touched by Var_Define().  */
	  symbolP->codeSize = 0;
	}
      gCurPObjP->d.macro.varListP = p;
    }
  
  Var_Define (ptr, len, type, true);
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
  const char c  = Input_Look ();
  if (c == 'L')
    type = ValueBool;
  else if (c == 'A')
    type = ValueInt;
  else if (c == 'S')
    type = ValueString;
  else
    return true;
  Input_Skip ();
  if (!Input_IsEndOfKeyword ())
    return true;

  switch (label->tag)
    {
      case LexNone:
	Error (ErrorError, "Label missing");
	return false;

      case LexId:
	break;

      case LexLocalLabel:
	Error (ErrorError, "Local label is not allowed here");
        return false;

      default:
	assert (0);
	break;
    }

  Symbol *sym = Symbol_Find (label);
  if (sym == NULL)
    {
      Error (ErrorError, "'%.*s' is undefined",
	     (int)label->Data.Id.len, label->Data.Id.str);
      return false;
    }
  if (type != sym->value.Tag)
    {
      Error (ErrorError, "Wrong type for symbol '%.*s'",
	     (int)label->Data.Id.len, label->Data.Id.str);
      return false;
    }
  const Value *value = Expr_BuildAndEval (sym->value.Tag);
  switch (value->Tag)
    {
      case ValueIllegal:
	Error (ErrorError, "Wrong variable type for '%.*s'", (int)label->Data.Id.len, label->Data.Id.str);
	break;

      default:
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
	  Value_Free (&p->symbolP->value);
	  *p->symbolP = p->symbol;
	}
      else
	{
	  const Lex l = Lex_Id (p->name, strlen (p->name));
	  Symbol_Remove (&l);
	}

      const VarPos *q = p->next;
      free ((void *)p);
      p = q;
    }
}
