/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2000-2014 GCCSDK Developers
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
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

/**
 * Created for each local variable encountered in a macro as we need to restore
 * it.
 */
typedef struct VarPos
{
  const struct VarPos *next;
  Symbol *symbolP; /**< Non-NULL when macro caller has this variable already defined.  */
  SymbolAttr symbolAttr; /**< When symbolP is non-NULL, the previous symbol's
    attribute set.  */
  char name[]; /**< NUL terminated symbol name.  */
} VarPos;

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
  Symbol *symP = Var_Define ("asasm$version", sizeof ("asasm$version")-1,
			     ValueInt, false);
  if (symP != NULL)
    symP->attr.value.Data.Int.i = ASASM_VERSION;
}

/**
 * \return User description for the value type a variable can contain.
 */
static const char *
Var_GetDescription (ValueTag type)
{
  assert (type == ValueInt || type == ValueString || type == ValueBool);
  const char *descP;
  switch (type)
    {
      case ValueInt:
	descP = "integer";
	break;
      case ValueString:
	descP = "string";
	break;
      case ValueBool:
	descP = "boolean";
	break;
    }
  return descP;
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
  assert (type == ValueInt || type == ValueString || type == ValueBool);

  /* Check if global variables are not already used as constants, area names,
     CPU/coprocessor name, etc.
     Local macro names can overrule any existing symbol.  */
  Symbol *sym;
  if (!localMacro && (sym = Symbol_Find (ptr, len)) != NULL)
    {
      if ((sym->attr.type & SYMBOL_RW) == 0)
	{
	  const char *symDescP = Symbol_GetDescription (sym);
	  Error (ErrorError, "Redefinition of %s %s as %s variable",
		 symDescP, sym->str, localMacro ? "local" : "global");
	  Error_Line (sym->attr.fileName, sym->attr.lineNumber, ErrorError, "note: %c%s %s was first used here",
		      toupper ((unsigned char)symDescP[0]), symDescP + 1, sym->str);
	  return NULL;
	}
    }
  else
    sym = Symbol_Get (ptr, len);
  
  if (sym->attr.value.Tag != ValueIllegal)
    {
      /* A local variable can have a different type than a similar named
	 global variable.  */
      /* FIXME: this is not 100% bullet proof: a local variable with same
	 name as global one will not get SYMBOL_MACRO_LOCAL bit so remains
	 global.  So we won't detect two different type local variables (with
	 same name) when we already have a global variable also with that
	 name).  */
      if (sym->attr.value.Tag != type
          && (!localMacro || (sym->attr.type & SYMBOL_MACRO_LOCAL) != 0))
	{
	  const char *symDescP = Symbol_GetDescription (sym);
	  Error (ErrorError, "Redefinition of %s %s %s as %s %s",
		 Var_GetDescription (sym->attr.value.Tag), symDescP, sym->str,
		 Var_GetDescription (type), symDescP);
	  Error_Line (sym->attr.fileName, sym->attr.lineNumber, ErrorError, "note: Previous definition was here");
	  return NULL;
	}

      /* When symbol is already known as global, it remains a global variable
	 (and :DEF: returns {TRUE} for it).  */
      localMacro = false;
    }

  sym->attr.type |= SYMBOL_DEFINED | SYMBOL_ABSOLUTE | SYMBOL_RW;
  if (localMacro)
    sym->attr.type |= SYMBOL_MACRO_LOCAL;

  Value_Free (&sym->attr.value);
  switch (type)
    {
      case ValueInt:
	sym->attr.value = Value_Int (0, eIntType_PureInt);
	break;

      case ValueBool:
	sym->attr.value = Value_Bool (false);
	break;

      case ValueString:
	sym->attr.value = Value_String (NULL, 0, false);
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
  Symbol *symbolP = Symbol_Find (ptr, len);

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
      /* Create a variable restore point.  */
      VarPos *varPosP;
      if ((varPosP = malloc (sizeof (VarPos) + len + 1)) == NULL)
	Error_OutOfMem ();
      memcpy (varPosP->name, ptr, len); varPosP->name[len] = '\0';
      varPosP->next = gCurPObjP->d.macro.varListP;
      if ((varPosP->symbolP = symbolP) != NULL)
	{
	  varPosP->symbolAttr = symbolP->attr;
	  varPosP->symbolAttr.value = Value_Copy (&symbolP->attr.value);
	  /* Reset Symbol parts which won't get touched by Var_Define().  */
	  symbolP->attr.codeSize = 0;
	}
      gCurPObjP->d.macro.varListP = varPosP;
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
  const char setType  = Input_Look ();
  if (setType == 'L')
    type = ValueBool;
  else if (setType == 'A')
    type = ValueInt;
  else if (setType == 'S')
    type = ValueString;
  else
    return true;
  Input_Skip ();
  if (!Input_IsEndOfKeyword ())
    return true;

  switch (label->tag)
    {
      case LexNone:
	Error (ErrorError, "Variable name missing");
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

  Symbol *sym = Symbol_Find (label->Data.Id.str, label->Data.Id.len);
  if (sym == NULL)
    {
      Error (ErrorError, "Variable %.*s is undefined",
	     (int)label->Data.Id.len, label->Data.Id.str);
      return false;
    }
  if ((sym->attr.type & SYMBOL_RW) == 0)
    {
      const char *symDescP = Symbol_GetDescription (sym);
      Error (ErrorError, "%c%s %s can not be used as variable",
	     toupper ((unsigned char)symDescP[0]), symDescP + 1, sym->str);
      Error_Line (sym->attr.fileName, sym->attr.lineNumber, ErrorError, "%c%s %s was defined here",
		  toupper ((unsigned char)symDescP[0]), symDescP + 1, sym->str);
      return false;
    }
  if (type != sym->attr.value.Tag)
    {
      const char *scope = (sym->attr.type & SYMBOL_MACRO_LOCAL) ? "local" : "global";
      Error (ErrorError, "SET%c is unexpected for %s %s variable %s",
	     setType, Var_GetDescription (sym->attr.value.Tag), scope, sym->str);
      Error_Line (sym->attr.fileName, sym->attr.lineNumber, ErrorError, "note: %c%s variable %s was defined here",
		  toupper ((unsigned char)scope[0]), scope + 1, sym->str);
      return false;
    }
  const Value *value = Expr_BuildAndEval (sym->attr.value.Tag);
  switch (value->Tag)
    {
      case ValueIllegal:
	Error (ErrorError, "Wrong value type for variable %s", sym->str);
	break;

      default:
	Value_Assign (&sym->attr.value, value);
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
	  Value_Free (&p->symbolP->attr.value);
	  p->symbolP->attr = p->symbolAttr;
	}
      else
	Symbol_Remove (p->name, strlen (p->name));

      const VarPos *q = p->next;
      free ((void *)p);
      p = q;
    }
}
