/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */

#include "config.h"

#include <assert.h>

#include "directive_symbol.h"
#include "error.h"
#include "expr.h"
#include "get.h"

/**
 * Define given symbol with what's located at the parser.  Fail when the
 * symbol is already defined with a value different than parsed.
 * \param legal OR'd Value* bits for each valid Value type.
 * \param intType Integer subtype when legal equals ValueInt.
 */
static bool
Define (const char *msg, Symbol *sym, ValueTag legal, IntType_e intType)
{
  bool failed = false;
  if (sym == NULL)
    {
      Error (ErrorError, "Missing label before %s", msg);
      failed = true;
    }
  else
    {
      const Value *value = Expr_BuildAndEval (legal);
      if (value->Tag == ValueIllegal)
	{
	  Error (ErrorError, "Illegal %s", msg);
	  failed = true;
	}
      else
	{
	  Value valueDef = *value;
	  if (valueDef.Tag == ValueInt && legal == ValueInt)
	    {
	      /* Only when we have a pure int coming from our expression
	         evaluator, we can overrule its type by what the caller
	         expects.
	         If it isn't a pure int, the type must be what the caller
	         expects.  */
	      if (valueDef.Data.Int.type == eIntType_PureInt)
		valueDef.Data.Int.type = intType;
	      else if (valueDef.Data.Int.type != intType)
		{
		  Error (ErrorError, "Not a %s", msg);
		  failed = true;
		}
	    }

	  if (!failed)
	    {
	      /* We allow export for pure integers, i.e. for the "Foo EQU 42"
	         case.
	         FIXME: what about 1 character strings "Foo EQU "x" ? */
	      unsigned symbolType;
	      if (valueDef.Tag == ValueInt && valueDef.Data.Int.type == eIntType_PureInt)
		symbolType = SYMBOL_CONSTANT | SYMBOL_ABSOLUTE;
	      else
		symbolType = SYMBOL_CONSTANT | SYMBOL_ABSOLUTE | SYMBOL_NO_EXPORT;
	      failed = Symbol_Define (sym, symbolType, &valueDef);
	    }
	}
    }

  return failed;
}

/**
 * Implements CN.
 */
bool
c_cn (Symbol *symP)
{
  if (!Define ("coprocessor register", symP, ValueInt, eIntType_CoProReg))
    {
      assert (symP->attr.value.Tag == ValueInt);
      int no = symP->attr.value.Data.Int.i;
      if (no < 0 || no > 15)
	{
	  symP->attr.value.Data.Int.i = 0;
	  Error (ErrorError, "Illegal %s register %d (using 0)", "coprocessor", no);
	}
    }
  return false;
}

/**
 * Implements CP.
 */
bool
c_cp (Symbol *symP)
{
  if (!Define ("coprocessor number", symP, ValueInt, eIntType_CoProNum))
    {
      assert (symP->attr.value.Tag == ValueInt);
      int no = symP->attr.value.Data.Int.i;
      if (no < 0 || no > 15)
	{
	  symP->attr.value.Data.Int.i = 0;
	  Error (ErrorError, "Illegal coprocessor number %d (using 0)", no);
	}
    }
  return false;
}

/**
 * Implements EQU and *.
 *   <symbol> EQU <expression>, <attribute>
 */
bool
c_equ (Symbol *symP)
{
  /* FIXME: <attribute> parsing.  */
  /* FIXME: ObjAsm limits EQU to ValueInt | ValueAddr | ValueSymbol, should
     we do the same ? */
  Define ("* or EQU", symP, ValueAll, 0);
  return false;
}

/**
 * Implements FN.
 */
bool
c_fn (Symbol *symP)
{
  if (!Define ("FPU register", symP, ValueInt, eIntType_FPU))
    {
      assert (symP->attr.value.Tag == ValueInt);
      int no = symP->attr.value.Data.Int.i;
      if (no < 0 || no > 7)
	{
	  symP->attr.value.Data.Int.i = 0;
	  Error (ErrorError, "Illegal %s register %d (using 0)", "FPU", no);
	}
    }
  return false;
}

/**
 * Implements RN.
 */
bool
c_rn (Symbol *symP)
{
  if (!Define ("CPU register", symP, ValueInt, eIntType_CPU))
    {
      assert (symP->attr.value.Tag == ValueInt);
      int no = symP->attr.value.Data.Int.i;
      if (no < 0 || no > 15)
	{
	  symP->attr.value.Data.Int.i = 0;
	  Error (ErrorError, "Illegal %s register %d (using 0)", "CPU", no);
	}
    }
  return false;
}

/**
 * Implements RLIST.
 */
bool
c_rlist (Symbol *symP)
{
  if (symP == NULL)
    {
      Error (ErrorError, "Missing label before %s", "register list");
      return false;
    }
  const Value rlistValue = Value_Int (Get_CPURList (), eIntType_CPURList);
  Symbol_Define (symP, SYMBOL_CONSTANT | SYMBOL_ABSOLUTE | SYMBOL_NO_EXPORT, &rlistValue);
  return false;
}
