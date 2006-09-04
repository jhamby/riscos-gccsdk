/*
 * AS an assembler for ARM
 * Copyright � 1992 Niklas R�jemo
 * Copyright (c) 2005-2006 GCCSDK Developers
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
 * lex.c
 */

#include "config.h"
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "area.h"
#include "error.h"
#include "help_lex.h"
#include "input.h"
#include "lex.h"
#include "local.h"
#include "main.h"
#include "os.h"
#include "symbol.h"


const char Pri[2][10] =
{
  {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},	/* AS */
  {1, 2, 3, 4, 10, 5, 6, 7, 8, 9}	/* ObjAsm? */
};

static Lex nextbinop;
static BOOL nextbinopvalid = FALSE;

/*
 * A simple and fast generic string hasher based on Peter K. Pearson's
 * article in CACM 33-6, pp. 677.

 s string to hash
 maxn maximum number of chars to consider
 hashs hash table size. */

static int hashstr (const char *s, int maxn)
{
  static const unsigned char hashtab[] =
  {
    1, 87, 49, 12, 176, 178, 102, 166, 121, 193, 6, 84, 249, 230, 44, 163,
    14, 197, 213, 181, 161, 85, 218, 80, 64, 239, 24, 226, 236, 142, 38, 200,
    110, 177, 104, 103, 141, 253, 255, 50, 77, 101, 81, 18, 45, 96, 31, 222,
    25, 107, 190, 70, 86, 237, 240, 34, 72, 242, 20, 214, 244, 227, 149, 235,
    97, 234, 57, 22, 60, 250, 82, 175, 208, 5, 127, 199, 111, 62, 135, 248,
    174, 169, 211, 58, 66, 154, 106, 195, 245, 171, 17, 187, 182, 179, 0, 243,
    132, 56, 148, 75, 128, 133, 158, 100, 130, 126, 91, 13, 153, 246, 216, 219,
    119, 68, 223, 78, 83, 88, 201, 99, 122, 11, 92, 32, 136, 114, 52, 10,
    138, 30, 48, 183, 156, 35, 61, 26, 143, 74, 251, 94, 129, 162, 63, 152,
    170, 7, 115, 167, 241, 206, 3, 150, 55, 59, 151, 220, 90, 53, 23, 131,
    125, 173, 15, 238, 79, 95, 89, 16, 105, 137, 225, 224, 217, 160, 37, 123,
    118, 73, 2, 157, 46, 116, 9, 145, 134, 228, 207, 212, 202, 215, 69, 229,
    27, 188, 67, 124, 168, 252, 42, 4, 29, 108, 21, 247, 19, 205, 39, 203,
    233, 40, 186, 147, 198, 192, 155, 33, 164, 191, 98, 204, 165, 180, 117, 76,
    140, 36, 210, 172, 41, 54, 159, 8, 185, 232, 113, 196, 231, 47, 146, 120,
    51, 65, 28, 144, 254, 221, 93, 189, 194, 139, 112, 43, 71, 109, 184, 209,
  };
  const unsigned char *p;
  unsigned char h = 0;
  int i, rh = 0;

  for (i = 0, p = (const unsigned char *) s; *p && i < maxn; i++, p++)
    h = hashtab[h ^ *p];

  rh = h;

  if (SYMBOL_TABLESIZE > 256 && *s)
    {
      for (i = 1, p = (const unsigned char *) s, h = (*p++ + 1) & 0xff;
	   *p && i < maxn; i++, p++)
	h = hashtab[h ^ *p];

      rh <<= 8;
      rh |= h;
    }
  return rh % SYMBOL_TABLESIZE;
}


static int
lexint (int base)
{
  int res;
  char c;

  if (base != 16)
    {
      if ((c = inputLook ()) == '0')
	{
	  if (!objasm)
	    base = 8;
	  if ((c = inputSkipLook ()) == 'x' || c == 'X')
	    {
	      base = 16;
	      inputSkip ();
	    }
	}
      else
	{
	  if (inputLookN (1) == '_')
	    {
	      inputSkipN (2);
	      if ((base = c - '0') < 2 || base > 9)
		error (ErrorError, TRUE, "Illegal base %d", base);
	    }
	}
    }

  for (res = 0; isxdigit (c = inputLookLower ()); inputSkip ())
    {
      c -= (c >= 'a') ? 'a' - 10 : '0';

      if (c >= base)
	return res;

      res = res * base + c;
    }

  return res;
}


static FLOAT
lexfloat (int r)
{
  FLOAT res = r;
  FLOAT frac = 0.1;
  FLOAT exp = 0.0;
  int signexp = 1;
  char c;

  if (inputGet () == '.')
    {				/* Fraction part */
      while (isdigit (c = inputGet ()))
	{
	  res = res + frac * ((FLOAT) c - (FLOAT) '0');
	  frac /= 10.0;
	}
      if (c == 'e' || c == 'E')
	{			/* Exponent part */
	  if (inputLook () == '-')
	    {
	      inputSkip ();
	      signexp = -1;
	    }
	  else if (inputLook () == '+')
	    {
	      inputSkip ();
	    }
	  while (isdigit (c = inputGet ()))
	    exp = exp * 10.0 + ((FLOAT) c - (FLOAT) '0');
	}
      inputUnGet (c);
    }
  else
    error (ErrorError, TRUE, "Internal lexfloat: parse error");
  return res * pow (10.0, signexp * exp);
}


static Lex
lexGetIdInternal (BOOL genError)
{
  char c;
  Lex result;
  nextbinopvalid = FALSE;

  skipblanks ();
  if ((c = inputGet ()) == '|')
    {
      result.tag = LexId;
      result.LexId.str = inputSymbol (&result.LexId.len, '|');
      if (inputGet () != '|')
	error (ErrorError, TRUE, "Identifier continues over newline");
      result.LexId.hash = hashstr (result.LexId.str, result.LexId.len);
    }
  else
    {
#if 0	/* inputUnGet validates operation against input_buff,
	which we may not be using at this point.
	*/
      inputUnGet (c);
#else
	inputPutBack( c );
#endif

      /* Note: we allow identifiers to begin with '#'.
       */
      if (isalpha (c) || c == '.' || c == '_' || c == '#')
	{
	  result.tag = LexId;
	  result.LexId.str = inputSymbol (&result.LexId.len, 0);
	  result.LexId.hash = hashstr (result.LexId.str, result.LexId.len);
	}
      else
	{
	  if (genError)
	    error (ErrorError, TRUE, "Missing identifier");
	  result.tag = LexNone;
	}
    }
  if (result.tag == LexId && result.LexId.len > 1)
    localMunge (&result);
  return result;
}


Lex
lexGetId (void)
{
  return lexGetIdInternal (TRUE);
}


Lex
lexGetIdNoError (void)
{
  return lexGetIdInternal (FALSE);
}


static char *
lexReadLocal (int *len, int *label)
{
  char *name;
  if (!isdigit (inputLook ()))
    {
      error (ErrorSerious, TRUE, "Missing local label number");

      return 0;
    }
  *label = inputGet () - '0';
  if (isdigit (inputLook ()))
    *label = (*label * 10) + (inputGet () - '0');
  name = inputSymbol (len, 0);
  if (len)
    if (strncmp (rout_id, name, *len))
      {
	error (ErrorError, TRUE, "Local label name (%s) does not match routine name (%s)", rout_id, name );
	return 0;
      }
  return name;
}


static Lex
lexMakeLocal (int dir)
{
  int label, len;
  int i = 0;
  Lex result;
  char *name, id[1024];
  result.tag = LexNone;
  name = lexReadLocal (&len, &label);
  if (!name)
    return result;
  switch (dir)
    {
    case -1:
      if ((i = rout_lblno[label] - 1) < 0)
	{
	  error (ErrorError, FALSE, "Missing local label (bwd) with ID %02i", label);
	  return result;
	}
      break;
    case 0:
      if ((i = rout_lblno[label] - 1) < 0)
        i++;
      break;
    case 1:
      i = rout_lblno[label];
      break;
    }
  sprintf (id, localFormat, areaCurrent, label, i, rout_id);
  result.LexId.str = strdup (id);
  result.LexId.len = strlen (id);
  result.LexId.hash = hashstr (result.LexId.str, result.LexId.len);
  if (!result.LexId.str)
    errorOutOfMem ("lexMakeLocal");
  else
    result.tag = LexId;
  return result;
}


Lex
lexGetLocal (void)
{
  Lex result;
  result.tag = LexNone;
  nextbinopvalid = FALSE;
  if (isdigit (inputLook ()))
    {
      char *name, id[1024];
      int len, label;
      name = lexReadLocal (&len, &label);
      if (!name)
	return result;
      sprintf (id, localFormat, areaCurrent, label, rout_lblno[label]++, rout_id);
      result.LexId.str = strdup (id);
      result.LexId.len = strlen (id);
      if (!result.LexId.str)
	{
	  errorOutOfMem ("lexGetLocal");
	  return result;
	}
      result.LexId.hash = hashstr (result.LexId.str, result.LexId.len);
      result.tag = LexId;
      return result;
    }
  return lexGetId ();
}





Lex
lexGetPrim (void)
{
  char c;
  char *str;
  int len;
  Lex result;
  nextbinopvalid = FALSE;
  skipblanks ();
  switch (c = inputGet ())
    {
    case '+':
      result.tag = LexOperator;
      result.LexOperator.op = Op_none;
      result.LexOperator.pri = PRI (10);
      break;
    case '-':
      result.tag = LexOperator;
      result.LexOperator.op = Op_neg;
      result.LexOperator.pri = PRI (10);
      break;
    case '!':
      result.tag = LexOperator;
      result.LexOperator.op = Op_lnot;
      result.LexOperator.pri = PRI (10);
      break;
    case '~':
      result.tag = LexOperator;
      result.LexOperator.op = Op_not;
      result.LexOperator.pri = PRI (10);
      break;
    case '?':
      error (ErrorError, TRUE, "Sorry, '?' not implemented");
    case '(':
    case ')':
      result.tag = LexDelim;
      result.LexDelim.delim = c;
      break;
    case ':':
      lexAcornUnop (&result);
      break;
    case '&':
      result.tag = LexInt;
      result.LexInt.value = lexint (16);
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      inputUnGet (c);
      result.tag = LexInt;
      result.LexInt.value = lexint (10);
      if (inputLook () == '.')
	{
	  result.tag = LexFloat;
	  result.LexFloat.value = lexfloat (result.LexInt.value);
	}
      break;
    case '\'':
      result.tag = LexInt;
      str = inputSymbol (&len, '\'');
      if (inputGet () != '\'')
	error (ErrorError, TRUE, "Character continues over newline");
      result.LexInt.value = lexChar2Int (TRUE, len, str);
      break;
    case '"':
      result.tag = LexString;
      str = inputSymbol (&len, '"');
      if ((str = strndup (str, len)) == NULL)
        {
          errorOutOfMem("lexGetPrim");
          break;
        }
      if (inputGet () != '"')
	error (ErrorError, TRUE, "String continues over newline");
      if (objasm)
	{
	  char *s1, *s2;
	  int l1;
	  while (inputLook () == '"')
	    {
	      inputSkip ();
	      s1 = inputSymbol (&l1, '"');
	      if (inputGet () != '"')
		{
		  error (ErrorError, TRUE, "String continues over newline");
		  break;
		}
	      if ((s2 = malloc (len + l1 + 1)) == NULL)
		{
		  errorOutOfMem ("macroAdd");
		  break;
		}
	      memcpy (s2, str, len);
	      s2[len] = '"';
	      memcpy (s2 + len + 1, s1, l1);
	      free (str);
	      str = s2;
	      len += l1 + 1;
	    }
	  /* now deal with \\ */
	  s1 = s2 = str;
	  while (*s1)
	    {
	      if (*s1 == '\\')
		{
		  switch (l1 = *++s1)
		    {
		    case '0':
		    case '1':
		    case '2':
		    case '3':
		    case '4':
		    case '5':
		    case '6':
		    case '7':
		      l1 = *s1++ - '0';
		      if (*s1 >= '0' && *s1 <= '7')
			{
			  l1 = l1 * 8 + *s1++ - '0';
			  if (*s1 >= '0' && *s1 <= '7')
			    l1 = l1 * 8 + *s1++ - '0';
			}
		      break;
		    case 'x':
		      if (isxdigit (*++s1))
			{	/* implied ASCII-like */
			  l1 = *s1 - '0' - 7 * (*s1 > '9') - 32 * (*s1 >= 'a');
			  if (isxdigit (*++s1))
			    {
			      l1 = l1 * 16 + *s1 - '0' - 7 * (*s1 > '9') - 32 * (*s1 >= 'a');
			      s1++;
			    }
			}
		      break;
		    case 'a':
		      l1 = 7;
		      s1++;
		      break;
		    case 'b':
		      l1 = 8;
		      s1++;
		      break;
		    case 'f':
		      l1 = 12;
		      s1++;
		      break;
		    case 'n':
		      l1 = 10;
		      s1++;
		      break;
		    case 'r':
		      l1 = 13;
		      s1++;
		      break;
		    case 't':
		      l1 = 9;
		      s1++;
		      break;
		    case 'v':
		      l1 = 11;
		      s1++;
		      break;
		    default:
		      s1++;
		      break;
		    }
		  *s2++ = l1;
		}
	      else
		*s2++ = *s1++;
	    }
	  *s2 = 0;
	  len -= s1 - s2;
	  str = realloc (str, len);	/* try to reduce size of block */
	}
      result.LexString.str = str;
      result.LexString.len = len;
      break;
    case '|':
      result.tag = LexId;
      result.LexId.str = inputSymbol (&result.LexId.len, '|');
      if (inputGet () != '|')
	error (ErrorError, TRUE, "Identifier continues over newline");
      result.LexId.hash = hashstr (result.LexId.str, result.LexId.len);

      break;
    case '.':
      result.tag = LexPosition;
      break;
    case '@':
      result.tag = gcc_backend ? LexNone : LexStorage;
      break;
    case '%':
      {
	int dir;
	switch (inputLookLower ())
	  {
	  case 'f':
	    inputSkip ();
	    dir = 1;
	    break;
	  case 'b':
	    inputSkip ();
	    dir = -1;
	    break;
	  default:
	    dir = 0;
	    break;
	  }
	switch (inputLookLower ())
	  {
	  case 't':
	    inputSkip ();

	    	if (pedantic)
		{
#if 0
	/* reason for ignoring flag clarified */
	    error (ErrorWarning, TRUE, "Ignored 't' in local label name");
#else
	    error (ErrorWarning, TRUE, "Range qualifiers in local label names are not implemented. " \
	    				"Ignored 't' in local label name");
#endif
		}
	    break;
	  case 'a':
	    inputSkip ();
	    break;
	  default:
	    if (pedantic)
		{
#if 0
/*	This warning is confusing to the user, as it warns of a deficiency in as.
	I've re-worded it accordingly.
	What really needs to be done is for range qualifiers to be implemented.
*/
	    error (ErrorWarning, TRUE, "Assumed 'a' in local label name");
#else
	    error (ErrorWarning, TRUE, "Range qualifiers in local label names are not implemented. " \
	    				"'all macro levels' is assumed");
#endif
		}
	    break;
	  }
	return lexMakeLocal (dir);
      }
      break;
    case '{':
      lexAcornPrim (&result);
      break;
    default:
      inputUnGet (c);
      if (isalpha (c) || c == '_')
	{
	  result.tag = LexId;
	  result.LexId.str = inputSymbol (&result.LexId.len, 0);
	  result.LexId.hash = hashstr (result.LexId.str, result.LexId.len);
	}
      else
	result.tag = LexNone;
      break;
    }
  if (result.tag == LexId && result.LexId.len > 1)
    localMunge (&result);
  return result;
}

Lex
lexGetBinop (void)
{
  Lex result;
  int c;
  if (nextbinopvalid)
    {
      nextbinopvalid = FALSE;
      return nextbinop;
    }
  skipblanks ();
  switch (c = inputGet ())
    {
    case '*':
      result.tag = LexOperator;
      result.LexOperator.op = Op_mul;
      result.LexOperator.pri = PRI (10);
      break;
    case '/':
      result.tag = LexOperator;
      result.LexOperator.op = Op_div;
      result.LexOperator.pri = PRI (10);
      break;
    case '%':
      result.tag = LexOperator;
      result.LexOperator.op = Op_mod;
      result.LexOperator.pri = PRI (10);
      break;
    case '+':
      result.tag = LexOperator;
      result.LexOperator.op = Op_add;
      result.LexOperator.pri = PRI (9);
      break;
    case '-':
      result.tag = LexOperator;
      result.LexOperator.op = Op_sub;
      result.LexOperator.pri = PRI (9);
      break;
    case '^':
      result.tag = LexOperator;
      result.LexOperator.op = Op_xor;
      result.LexOperator.pri = PRI (6);
      break;
    case '>':
      result.tag = LexOperator;
      switch (inputLook ())
	{
	case '>':
	  result.LexOperator.pri = PRI (5);
	  if (inputSkipLook () == '>')
	    {
	      inputSkip ();
	      result.LexOperator.op = Op_asr;
	    }
	  else
	    {
	      result.LexOperator.op = Op_sr;
	    }
	  break;
	case '=':
	  inputSkip ();
	  result.LexOperator.op = Op_ge;
	  result.LexOperator.pri = PRI (4);
	  break;
	default:
	  result.LexOperator.op = Op_gt;
	  result.LexOperator.pri = PRI (4);
	  break;
	}
      break;
    case '<':
      result.tag = LexOperator;
      switch (inputLook ())
	{
	case '<':
	  inputSkip ();
	  result.LexOperator.op = Op_sl;
	  result.LexOperator.pri = PRI (5);
	  break;
	case '=':
	  inputSkip ();
	  result.LexOperator.op = Op_le;
	  result.LexOperator.pri = PRI (4);
	  break;
	case '>':		/* fixed to allow "<>" as well as "!=" */
	  inputSkip ();
	  result.LexOperator.op = Op_ne;
	  result.LexOperator.pri = PRI (3);
	  break;
	default:
	  result.LexOperator.op = Op_lt;
	  result.LexOperator.pri = PRI (4);
	  break;
	}
      break;
    case '=':
      if (inputLook () == '=')
	inputSkip ();		/* fixed to allow "=" or "==" */
      result.tag = LexOperator;
      result.LexOperator.pri = PRI (3);
      result.LexOperator.op = Op_eq;
      break;
    case '!':
      if (inputLook () == '=')
	{
	  inputSkip ();
	  result.tag = LexOperator;
	  result.LexOperator.pri = PRI (3);
	  result.LexOperator.op = Op_ne;
	}
      else
	result.tag = LexNone;
      break;
    case '|':
      result.tag = LexOperator;
      if (inputLook () == '|')
	{
	  inputSkip ();
	  result.LexOperator.pri = PRI (1);
	  result.LexOperator.op = Op_lor;
	}
      else
	{
	  result.LexOperator.pri = PRI (7);
	  result.LexOperator.op = Op_or;
	}
      break;
    case '&':
      result.tag = LexOperator;
      if (inputLook () == '&')
	{
	  inputSkip ();
	  result.LexOperator.pri = PRI (2);
	  result.LexOperator.op = Op_land;
	}
      else
	{
	  result.LexOperator.pri = PRI (8);
	  result.LexOperator.op = Op_and;
	}
      break;
    case ':':
      lexAcornBinop (&result);
      break;
    default:
      inputUnGet (c);
      result.tag = LexNone;
      break;
    }
  return result;
}

int
lexNextPri ()
{
  if (!nextbinopvalid)
    {
      nextbinop = lexGetBinop ();
      nextbinopvalid = TRUE;
    }
  if (nextbinop.tag == LexOperator)
    return nextbinop.LexOperator.pri;
  else
    return -1;
}

Lex
lexTempLabel (const char *ptr, int len)
{
  Lex var;

  var.tag = LexId;
  var.LexId.str = ptr;
  var.LexId.len = len;
  var.LexId.hash = hashstr (ptr, len);
  return var;
}

#ifdef DEBUG
void
lexPrint(const Lex *lex)
{
  if (lex == NULL)
    {
      printf("<NULL> ");
      return;
    }
  switch (lex->tag)
    {
    case LexId:
      printf("Id <%.*s> ", lex->LexId.len, lex->LexId.str);
      break;
    case LexString:
      printf("Str <%.*s> ", lex->LexString.len, lex->LexString.str);
      break;
    case LexInt:
      printf("Int <%d> ", lex->LexInt.value);
      break;
    case LexFloat:
      printf("Flt <%g> ", lex->LexFloat.value);
      break;
    case LexOperator:
      printf("Op <%d, %d> ", lex->LexOperator.op, lex->LexOperator.pri);
      break;
    case LexPosition:
      printf("Pos ");
      break;
    case LexStorage:
      printf("Stor ");
      break;
    case LexDelim:
      printf("Delim <%d> ", lex->LexDelim.delim);
      break;
    case Lex00Label:
      printf("Label <%d> ", lex->Lex00Label.value);
      break;
    case LexBool:
      printf("Bool <%d> ", lex->Lex00Label.value);
      break;
    case LexNone:
      printf("None ");
      break;
    default:
      printf("Unknown lex tag 0x%x ", lex->tag);
      break;
    }
}
#endif
