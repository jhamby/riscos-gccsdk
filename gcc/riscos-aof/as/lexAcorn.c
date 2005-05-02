/*
 * AS an assembler for ARM
 * Copyright © 1992 Niklas Röjemo
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

#include "decode.h"
#include "input.h"
#include "lex.h"
#include "main.h"

#define FINISH_STR(string,Op,Pri) \
  if(notinput(string)) goto illegal; \
  lex->LexOperator.op = Op; lex->LexOperator.pri = PRI(Pri); return 1

#define FINISH_CHR(Op,Pri) \
  if(inputGet()!=':') goto illegal; \
  lex->LexOperator.op = Op; lex->LexOperator.pri = PRI(Pri); return 1;

int
lexAcornUnop (Lex * lex)
{
  lex->tag = LexOperator;
  switch (inputGetLower ())
    {
    case 'c':
      FINISH_STR ("hr:", Op_chr, 10);
    case 'd':
      if (notinput ("ef:"))
	goto illegal;
      *lex = lexGetPrim ();
      if (lex->tag != LexId)
	goto illegal;
      lex->LexInt.value = symbolFind (lex) != NULL;
      lex->tag = LexBool;
      return 1;
    case 'f':
      switch (inputGetLower ())
	{
	case 'a':
	  FINISH_STR ("ttr:", Op_fattr, 10);
	case 'e':
	  FINISH_STR ("xec", Op_fexec, 10);
	case 'l':
	  FINISH_STR ("oad:", Op_fload, 10);
	case 's':
	  FINISH_STR ("ize:", Op_fsize, 10);
	}
      break;
    case 'i':
      FINISH_STR ("ndex:", Op_index, 10);
    case 'l':
      switch (inputGetLower ())
	{
	case 'e':
	  FINISH_STR ("n:", Op_len, 10);
	case 'n':
	  FINISH_STR ("ot:", Op_lnot, 10);
	}
    case 'n':
      FINISH_STR ("ot:", Op_not, 10);
    case 's':
      FINISH_STR ("tr:", Op_str, 10);
    default:
    illegal:;
      break;
    }
  lex->tag = LexNone;
  return 0;
}


int
lexAcornBinop (Lex * lex)
{
  lex->tag = LexOperator;
  switch (inputGetLower ())
    {
    case 'a':
      FINISH_STR ("nd:", Op_and, 8);
    case 'c':
      FINISH_STR ("c:", Op_concat, 9);
    case 'e':
      FINISH_STR ("or:", Op_xor, 6);
    case 'l':
      switch (inputGetLower ())
	{
	case 'a':
	  FINISH_STR ("nd:", Op_land, 2);
	case 'e':
	  switch (inputGetLower ())
	    {
	    case 'f':
	      FINISH_STR ("t:", Op_left, 10);
	    case 'o':
	      FINISH_STR ("r:", Op_ne, 1);
	    }
	case 'o':
	  FINISH_STR ("r:", Op_lor, 1);
	}
      break;
    case 'm':
      FINISH_STR ("od:", Op_mod, 10);
    case 'o':
      FINISH_STR ("r:", Op_or, 7);
    case 'r':
      switch (inputGetLower ())
	{
	case 'i':
	  FINISH_STR ("ght:", Op_right, 10);
	case 'o':
	  switch (inputGetLower ())
	    {
	    case 'l':
	      FINISH_CHR (Op_rol, 5);
	    case 'r':
	      FINISH_CHR (Op_ror, 5);
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
	      FINISH_CHR (Op_sl, 5);
	    case 'r':
	      FINISH_CHR (Op_sr, 5);
	    }
	  break;
	}
      break;
    default:
    illegal:;
      break;
    }
  lex->tag = LexNone;
  return 0;
}


#define FINISH_STR_PRIM(string) if(notinput(string)) goto illegal;


int
lexAcornPrim (Lex * lex)
{
  switch (inputGetLower ())
    {
    case 'c':
      FINISH_STR_PRIM ("onfig}");
      lex->tag = LexInt;
      lex->LexInt.value = apcs_32bit ? 32 : 26;
      return 1;
    case 'f':
      FINISH_STR_PRIM ("alse}");
      lex->tag = LexBool;
      lex->LexInt.value = FALSE;
      return 1;
    case 'p':
      FINISH_STR_PRIM ("c}");
      lex->tag = LexPosition;
      return 1;
    case 's':
      FINISH_STR_PRIM ("oftfloat}");
      lex->tag = LexBool;
      lex->LexInt.value = apcs_softfloat;
      return 1;
    case 't':
      FINISH_STR_PRIM ("rue}");
      lex->tag = LexBool;
      lex->LexInt.value = TRUE;
      return 1;
    case 'v':
      FINISH_STR_PRIM ("ar}");
      lex->tag = LexStorage;
      return 1;
    default:
    illegal:;
      break;
    }
  lex->tag = LexNone;
  return 0;
}
