/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2000-2012 GCCSDK Developers
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
 * directive_symbol.c
 */

#include "config.h"

#include "directive_symbol.h"
#include "error.h"
#include "expr.h"

/**
 * Define given symbol with what's located at the parser.  Fail when the
 * symbol is already defined with a value different than parsed.
 */
static bool
Define (const char *msg, Symbol *sym, unsigned symType, ValueTag legal)
{
  bool fail;
  if (sym == NULL)
    {
      error (ErrorError, "Missing label before %s", msg);
      fail = true;
    }
  else
    {
      const Value *value = exprBuildAndEval (legal);
      if (value->Tag == ValueIllegal)
	{
	  error (ErrorError, "Illegal %s", msg);
	  fail = true;
	}
      else
	fail = Symbol_Define (sym, SYMBOL_ABSOLUTE | symType, value);
    }
  return fail;
}

/**
 * Implements CN.
 */
bool
c_cn (Symbol *symbol)
{
  if (!Define ("coprocessor register", symbol, SYMBOL_COPREG, ValueInt))
    {
      int no = symbol->value.Data.Int.i;
      if (no < 0 || no > 15)
	{
	  symbol->value.Data.Int.i = 0;
	  error (ErrorError, "Illegal %s register %d (using 0)", "coprocessor", no);
	}
    }
  return false;
}

/**
 * Implements CP.
 */
bool
c_cp (Symbol *symbol)
{
  if (!Define ("coprocessor number", symbol, SYMBOL_COPNUM, ValueInt))
    {
      int no = symbol->value.Data.Int.i;
      if (no < 0 || no > 15)
	{
	  symbol->value.Data.Int.i = 0;
	  error (ErrorError, "Illegal coprocessor number %d (using 0)", no);
	}
    }
  return false;
}

/**
 * Implements EQU and *.
 */
bool
c_equ (Symbol *symbol)
{
  Define ("* or EQU", symbol, 0, ValueAll);
  return false;
}

/**
 * Implements FN.
 */
bool
c_fn (Symbol *symbol)
{
  if (!Define ("float register", symbol, SYMBOL_FPUREG, ValueInt))
    {
      int no = symbol->value.Data.Int.i;
      if (no < 0 || no > 7)
	{
	  symbol->value.Data.Int.i = 0;
	  error (ErrorError, "Illegal %s register %d (using 0)", "FPU", no);
	}
    }
  return false;
}

/**
 * Implements RN.
 */
bool
c_rn (Symbol *symbol)
{
  if (!Define ("register", symbol, SYMBOL_CPUREG, ValueInt))
    {
      int no = symbol->value.Data.Int.i;
      if (no < 0 || no > 15)
	{
	  symbol->value.Data.Int.i = 0;
	  error (ErrorError, "Illegal %s register %d (using 0)", "CPU", no);
	}
    }
  return false;
}
