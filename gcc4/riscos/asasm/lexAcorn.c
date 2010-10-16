/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
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
 * lexAcorn.c
 */

#include "config.h"
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#include <string.h>

#include "decode.h"
#include "error.h"
#include "input.h"
#include "lex.h"
#include "lexAcorn.h"
#include "main.h"

#define FINISH_STR(string,Op,Pri)	\
  if (notinput(string))			\
    goto illegal;			\
  lex->LexOperator.op = Op;		\
  lex->LexOperator.pri = PRI(Pri);	\
  return;

#define FINISH_CHR(Op,Pri)		\
  if (inputGet()!=':')			\
    goto illegal;			\
  lex->LexOperator.op = Op;		\
  lex->LexOperator.pri = PRI(Pri);	\
  return;

void
lexAcornUnop (Lex *lex)
{
  lex->tag = LexOperator;
  switch (inputGetLower ())
    {
      case 'c':
	FINISH_STR ("hr:", Op_chr, 10); /* :chr: */
      case 'd':
	if (notinput ("ef:")) /* :def: */
	  goto illegal;
	*lex = lexGetPrim ();
	if (lex->tag != LexId)
	  goto illegal;
	/* :def: only returns {true} when the symbol is defined and it is not
	   a macro local variable.  */
	const Symbol *symP = symbolFind (lex);
	lex->LexInt.value =  symP != NULL && !(symP->type & SYMBOL_MACRO_LOCAL);
	lex->tag = LexBool;
	return;
      case 'f':
	switch (inputGetLower ())
	  {
	    case 'a':
	      FINISH_STR ("ttr:", Op_fattr, 10); /* :fattr: */
	    case 'e':
	      FINISH_STR ("xec:", Op_fexec, 10); /* :fexec: */
	    case 'l':
	      FINISH_STR ("oad:", Op_fload, 10); /* :fload: */
	    case 's':
	      FINISH_STR ("ize:", Op_fsize, 10); /* :fsize: */
	  }
	break;
      case 'i':
	FINISH_STR ("ndex:", Op_index, 10); /* :index: */
      case 'l':
	switch (inputGetLower ())
	  {
	    case 'e':
	      FINISH_STR ("n:", Op_len, 10); /* :len: */
	    case 'n':
	      FINISH_STR ("ot:", Op_lnot, 10); /* :lnot: */
	  }
	break;
      case 'n':
	FINISH_STR ("ot:", Op_not, 10); /* :not: */
      case 's':
	FINISH_STR ("tr:", Op_str, 10); /* :str: */
    }

illegal:
  lex->tag = LexNone;
}


void
lexAcornBinop (Lex * lex)
{
  lex->tag = LexOperator;
  switch (inputGetLower ())
    {
      case 'a':
	FINISH_STR ("nd:", Op_and, 8); /* :and: */
      case 'c':
	FINISH_STR ("c:", Op_concat, 9); /* :cc: */
      case 'e':
	FINISH_STR ("or:", Op_xor, 6); /* :eor: */
      case 'l':
	switch (inputGetLower ())
	  {
	    case 'a':
	      FINISH_STR ("nd:", Op_land, 2); /* :land: */
	    case 'e':
	      switch (inputGetLower ())
		{
		  case 'f':
		    FINISH_STR ("t:", Op_left, 10); /* :left: */
		  case 'o':
		    FINISH_STR ("r:", Op_ne, 1); /* :leor: */
		}
	      break;
	    case 'o':
	      FINISH_STR ("r:", Op_lor, 1); /* :lor: */
	  }
	break;
    case 'm':
	FINISH_STR ("od:", Op_mod, 10); /* :mod: */
    case 'o':
	FINISH_STR ("r:", Op_or, 7); /* :or: */
    case 'r':
	switch (inputGetLower ())
	  {
	    case 'i':
	      FINISH_STR ("ght:", Op_right, 10); /* :right: */
	    case 'o':
	      switch (inputGetLower ())
		{
		  case 'l':
		    FINISH_CHR (Op_rol, 5); /* :rol: */
		  case 'r':
		    FINISH_CHR (Op_ror, 5); /* :ror: */
		}
	      break;
	  }
	break;
    case 's':
	switch (inputGetLower ())
	  {
	    case 'h':
	      switch (inputGetLower ())
		{
		  case 'l':
		    FINISH_CHR (Op_sl, 5); /* :shl: */
		  case 'r':
		    FINISH_CHR (Op_sr, 5); /* :shr: */
		}
	      break;
	  }
	break;
    }

illegal:
  lex->tag = LexNone;
}


#define FINISH_STR_PRIM(string)	\
  if (notinput(string))		\
    goto illegal;


void
lexAcornPrim (Lex *lex)
{
  switch (inputGetLower ())
    {
      case 'c':
	FINISH_STR_PRIM ("onfig}"); /* {config} */
	lex->tag = LexInt;
	lex->LexInt.value = option_apcs_32bit ? 32 : 26;
	return;
      case 'e':
	FINISH_STR_PRIM ("ndian}"); /* {endian} */
	lex->tag = LexString;
	if ((lex->LexString.str = strdup ("little")) == NULL)
	  errorOutOfMem ();
	lex->LexString.len = sizeof ("little")-1;
	return;
      case 'f':
	FINISH_STR_PRIM ("alse}"); /* {false} */
	lex->tag = LexBool;
	lex->LexInt.value = false;
	return;
      case 'm':
	FINISH_STR_PRIM ("odule}"); /* {module} */
	lex->tag = LexBool;
	lex->LexInt.value = option_rma_module;
	return;
      case 'p':
	FINISH_STR_PRIM ("c}"); /* {pc} */
	lex->tag = LexPosition;
	return;
      case 's':
	FINISH_STR_PRIM ("oftfloat}"); /* {softfloat} */
	lex->tag = LexBool;
	lex->LexInt.value = option_apcs_softfloat;
	return;
      case 't':
	FINISH_STR_PRIM ("rue}"); /* {true} */
	lex->tag = LexBool;
	lex->LexInt.value = true;
	return;
      case 'v':
	FINISH_STR_PRIM ("ar}"); /* {var} */
	lex->tag = LexStorage;
	return;
      case 'o':
	FINISH_STR_PRIM ("pt}"); /* {opt} */
	lex->tag = LexInt;
	lex->LexInt.value = 2;
	return;
      case 'a':
	FINISH_STR_PRIM ("sasm}"); /* {asasm} */
	lex->tag = LexBool;
	lex->LexInt.value = true;
	return;
    }

illegal:
  lex->tag = LexNone;
}
