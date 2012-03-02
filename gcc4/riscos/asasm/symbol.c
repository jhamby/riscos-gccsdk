/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
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
 * symbol.c
 */

#include "config.h"

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#endif
#if HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "aoffile.h"
#include "area.h"
#include "code.h"
#include "elf.h"
#include "error.h"
#include "expr.h"
#include "global.h"
#include "input.h"
#include "local.h"
#include "main.h"
#include "output.h"
#include "symbol.h"

#ifdef DEBUG
#  define DEBUG_SYMBOL
#endif

static Symbol *symbolTable[SYMBOL_TABLESIZE];
static bool oKeepAllSymbols;
static bool oAllExportSymbolsAreWeak; /* FIXME: support this.  */

static Symbol *
symbolNew (const char *str, size_t len)
{
  Symbol *result;
  if ((result = (Symbol *) malloc (sizeof (Symbol) + len)) == NULL)
    errorOutOfMem ();
  result->next = NULL;
  result->type = result->offset = 0;
  result->value.Tag = ValueIllegal;
  result->codeSize = 0;
  result->area.rel /* = result->area.info */ = NULL;
  result->areaDef = areaCurrentSymbol;
  result->used = -1;
  result->len = len;
  memcpy (result->str, str, len);
  result->str[len] = 0;
  return result;
}

static void
symbolFree (Symbol **symPP)
{
  register Symbol *symP = *symPP;
  if (symP)
    {
      *symPP = symP->next;
      valueFree (&symP->value);
      free (symP);
    }
}

static bool
EqSymLex (const Symbol *str, const Lex *lx)
{
  if (str->len != lx->Data.Id.len)
    return false;
  return !memcmp (str->str, lx->Data.Id.str, str->len);
}


/**
 * \return Always a non-NULL value pointing to symbol representing given Lex
 * object.
 */
Symbol *
symbolGet (const Lex *l)
{
  assert (l->tag == LexId && "Internal symbolGet: non-ID");

  Symbol **isearch;
  for (isearch = &symbolTable[l->Data.Id.hash]; *isearch; isearch = &(*isearch)->next)
    {
      if (EqSymLex (*isearch, l))
	return *isearch;
    }

  *isearch = symbolNew (l->Data.Id.str, l->Data.Id.len);
  return *isearch;
}


Symbol *
symbolFind (const Lex *l)
{
  if (l->tag != LexId)
    return NULL;

  Symbol **isearch;
  for (isearch = &symbolTable[l->Data.Id.hash]; *isearch; isearch = &(*isearch)->next)
    {
      if (EqSymLex (*isearch, l))
	return *isearch;
    }
  return NULL;
}


/**
 * Defines given symbol with given value and type.  Only to be used for symbols
 * which can not be (or not supposed to be) redefined as this checks on
 * redefinition with different value or inconsistencies like area vs area label
 * definitions.
 * \return false if successful, true otherwise.
 */
bool
Symbol_Define (Symbol *symbol, unsigned newSymbolType, const Value *newValue)
{
  if (symbol->type & SYMBOL_AREA)
    {
      error (ErrorError, "Area label %s can not be redefined", symbol->str);
      return true;
    }
  assert ((newSymbolType & SYMBOL_AREA) == 0 && "Not to be used for defining area symbols");

  Value newValueCopy = { .Tag = ValueIllegal };
  if (symbol->type & SYMBOL_DEFINED)
    {
      if (symbol->areaDef
          && symbol->areaDef != areaCurrentSymbol
          && (symbol->type & SYMBOL_ABSOLUTE) == 0)
        {
          error (ErrorError, "Symbol %s is already defined in area %s", symbol->str, symbol->areaDef->str);
          return true;
        }
      if (symbol->value.Tag != ValueIllegal)
	{
	  bool diffValue;
	  if (valueEqual (&symbol->value, newValue))
	    diffValue = false;
	  else
	    {
	      if (symbol->value.Tag == ValueSymbol || newValue->Tag == ValueSymbol)
		{
		  /* newValue might point into our code array.  */
		  Value_Assign (&newValueCopy, newValue);
		  newValue = &newValueCopy;

		  codeInit ();
		  codeValue (&symbol->value, false);
		  Value val1 = { .Tag = ValueIllegal };
		  Value_Assign (&val1, exprEval (ValueAll));
		  codeInit ();
		  codeValue (newValue, false);
		  Value val2 = { .Tag = ValueIllegal };
		  Value_Assign (&val2, exprEval (ValueAll));
		  diffValue = !valueEqual (&val1, &val2);
#ifdef DEBUG
		  if (diffValue)
		    {
		      printf ("Diff: ");
		      valuePrint (&val1);
		      printf (" vs ");
		      valuePrint (&val2);
		      printf ("\n");
		    }
#endif
		  valueFree (&val1);
		  valueFree (&val2);
		}
	      else
		{
		  diffValue = true; /* Not sure if we don't have to try harder here.  */
#ifdef DEBUG
		  printf ("Diff: ");
		  valuePrint (&symbol->value);
		  printf (" vs ");
		  valuePrint (newValue);
		  printf ("\n");
#endif
		}
	    }
	  if (diffValue)
	    {
	      error (ErrorError, "Symbol %s can not be redefined with a different value", symbol->str);
	      return true;
	    }
	}
    }
  symbol->type |= newSymbolType | SYMBOL_DEFINED;
  symbol->areaDef = areaCurrentSymbol;
  Value_Assign (&symbol->value, newValue);

  if (newValue == &newValueCopy)
    valueFree (&newValueCopy);
  return false;
}


/**
 * Removes symbol from symbol table.
 */
void
symbolRemove (const Lex *l)
{
  assert (l->tag == LexId && "Internal symbolRemove: non-ID");

  for (Symbol **isearch = &symbolTable[l->Data.Id.hash];
       *isearch != NULL;
       isearch = &(*isearch)->next)
    {
      if (EqSymLex (*isearch, l))
	{
          symbolFree (isearch);
	  return;
	}
    }

  error (ErrorAbort, "Internal error: symbolRemove");
}


static bool
NeedToOutputSymbol (const Symbol *sym)
{
  /* All area symbols are exported except the implicit one.  */
  if (sym->type & SYMBOL_AREA)
    return !Area_IsImplicit (sym);

  /* All mapping symbols are exported as well.  */
  if (Area_IsMappingSymbol (sym->str))
    return true;

  bool doOutput = (((oKeepAllSymbols || (sym->type & SYMBOL_KEEP)) && (sym->value.Tag == ValueBool || sym->value.Tag == ValueInt))
		   || SYMBOL_KIND(sym->type) == SYMBOL_GLOBAL
                   || (SYMBOL_KIND(sym->type) == SYMBOL_REFERENCE && sym->used >= 0)
		  )
		  && !Local_IsLocalLabel (sym->str);
  return doOutput;
}


static int
SymbolCompare (const void *symPP1, const void *symPP2)
{
  const Symbol *symP1 = *(const Symbol **)symPP1;
  const Symbol *symP2 = *(const Symbol **)symPP2;

  /* Not sure how the mapping symbols for AOF output needs to be sorted.  */
  bool isMappingSym1 = Area_IsMappingSymbol (symP1->str);
  bool isMappingSym2 = Area_IsMappingSymbol (symP2->str);
  if (!option_aof || (!isMappingSym1 && !isMappingSym2)) 
    return strcasecmp (symP1->str, symP2->str);

  /* AOF: mapping symbols at the end of symbol table.  */
  if (isMappingSym1 != isMappingSym2)
    return isMappingSym1 ? 1 : -1;
  
  /* AOF: mapping symbols first sorted according to their area name.  */
  if (symP1->areaDef != symP2->areaDef)
    return strcasecmp (symP1->areaDef->str, symP2->areaDef->str);

  /* AOF: then sort according to their area offset value (don't sort based
     on mapping symbol type).  */
  int v1;
  switch (symP1->value.Tag)
    {
      case ValueAddr:
	v1 = symP1->value.Data.Addr.i;
	break;

      case ValueInt:
	v1 = symP1->value.Data.Int.i;
	break;

      case ValueSymbol:
	v1 = symP1->value.Data.Symbol.offset;
	break;

      default:
	v1 = 0;
	assert (0);
	break;
    }
  int v2;
  switch (symP2->value.Tag)
    {
      case ValueAddr:
	v2 = symP2->value.Data.Addr.i;
	break;

      case ValueInt:
	v2 = symP2->value.Data.Int.i;
	break;

      case ValueSymbol:
	v2 = symP2->value.Data.Symbol.offset;
	break;

      default:
	v2 = 0;
	assert (0);
	break;
    }
  return v1 - v2;
}


SymbolOut_t
Symbol_CreateSymbolOut (void)
{
  SymbolOut_t result =
    {
      .allSymbolsPP = NULL,
      .numAllSymbols = 0,
      .numLocalSymbols = 0,
      .stringSize = 0
    };

  /* Figure out how many symbols we want to output.  */
  for (unsigned i = 0; i != SYMBOL_TABLESIZE; ++i)
    {
      for (Symbol *sym = symbolTable[i]; sym; sym = sym->next)
	{
	  /* At this point sym->used is -1 when it is not needed for
	     relocation, either is 0 when used for relocation.  */
	  assert (sym->used == -1 || sym->used == 0);
	  if (sym->type & SYMBOL_AREA)
	    {
	      /* All AREA symbols are local ones.  */
	      assert (SYMBOL_KIND (sym->type) == 0);
	    }
	  else
	    {
	      if (sym->used == -1)
		{
		  /* Check for undefined exported and unused imported symbols.  */
		  if (SYMBOL_KIND (sym->type) == SYMBOL_REFERENCE)
		    {
		      if (Area_IsImplicit (sym->areaDef))
			errorLine (NULL, 0, ErrorWarning, "Symbol %s is imported but not used, or exported but not defined", sym->str);
		      else
			errorLine (NULL, 0, ErrorWarning, "In area %s, symbol %s is imported but not used, or exported but not defined", sym->areaDef->str, sym->str);
		    }
		}
	      else if (SYMBOL_KIND (sym->type) == 0)
		{
		  /* Make it a reference symbol.  */
		  sym->type |= SYMBOL_REFERENCE;
		  errorLine (NULL, 0, ErrorWarning, "Symbol %s is implicitly imported", sym->str);
		}
	    }
	  if (NeedToOutputSymbol (sym))
	    {
	      ++result.numAllSymbols;
	      if ((sym->type & SYMBOL_AREA) || SYMBOL_KIND (sym->type) == SYMBOL_LOCAL)
		++result.numLocalSymbols;
	      sym->used = 0;
	    }
          else
	    sym->used = -1;
	}
    }

  /* Claim space.  */
  if ((result.allSymbolsPP = malloc (result.numAllSymbols * sizeof (Symbol *))) == NULL)
    errorOutOfMem ();

  /* Run over symbols again, and start assigning them in our symbol output
     array.  */
  const unsigned localSymbolIndexStart = (option_aof) ? result.numAllSymbols - result.numLocalSymbols : 0;
  const unsigned globalSymbolIndexStart = (option_aof) ? 0 : result.numLocalSymbols;
  unsigned localSymbolIndex = localSymbolIndexStart;
  unsigned globalSymbolIndex = globalSymbolIndexStart;
  for (unsigned i = 0; i != SYMBOL_TABLESIZE; ++i)
    {
      for (Symbol *sym = symbolTable[i]; sym; sym = sym->next)
	{
	  if (sym->used < 0)
	    continue;
	  if ((sym->type & SYMBOL_AREA) || SYMBOL_KIND (sym->type) == SYMBOL_LOCAL)
	    result.allSymbolsPP[localSymbolIndex++] = sym;
	  else
	    result.allSymbolsPP[globalSymbolIndex++] = sym;
	}
    }
  assert ((option_aof && localSymbolIndex == result.numAllSymbols && globalSymbolIndex == localSymbolIndexStart)
          ^ (!option_aof && localSymbolIndex == globalSymbolIndexStart && globalSymbolIndex == result.numAllSymbols));

  /* Sort local and global symbols individually.  */
  qsort (&result.allSymbolsPP[localSymbolIndexStart], result.numLocalSymbols, sizeof (Symbol *), SymbolCompare);
  qsort (&result.allSymbolsPP[globalSymbolIndexStart], result.numAllSymbols - result.numLocalSymbols, sizeof (Symbol *), SymbolCompare);

  /* Assign Symbol::offset.
     We want to limit output the strings of mapping symbol to the first two
     characters so we can share their string output.
     Also collect the final string size needed for our output symbols.  */
  struct
    {
      unsigned offset;
    } mapSymbols[3] =
    {
      { UINT_MAX }, /* $a */
      { UINT_MAX }, /* $d */
      { UINT_MAX }  /* $t */
    };
  for (unsigned symbolIndex = 0; symbolIndex != result.numAllSymbols; ++symbolIndex)
    {
      Symbol *sym = result.allSymbolsPP[symbolIndex];
      if (Area_IsMappingSymbol (sym->str))
	{
	  assert (!(sym->type & SYMBOL_AREA));
	  int mappingSymbolindex = sym->str[1] == 'a' ? 0 : sym->str[1] == 'd' ? 1 : 2;
	  if (mapSymbols[mappingSymbolindex].offset == UINT_MAX)
	    {
	      /* First time we see this particular mapping symbol.  */
	      mapSymbols[mappingSymbolindex].offset = sym->offset = result.stringSize;
	      result.stringSize += sizeof ("$d"); /* $d, $a, $t */
	    }
	  else
	    sym->offset = mapSymbols[mappingSymbolindex].offset;
	}
      else
	{
	  /* For ELF, section names are not mentioned in the string table
	     but part of the section header.  */
	  if ((sym->type & SYMBOL_AREA) && !option_aof)
	    sym->offset = 0;
	  else
	    {
	      sym->offset = result.stringSize;
	      result.stringSize += sym->len + 1;
	    }
	}
      /* Symbol::used now contains a number which can be used (for the choosen
         output format) to refer to that symbol.
	 For AOF: if it is an area symbol, it's its area number. For all other
	          symbols, the index in the symbol table.
	 For ELF: the index in symbol table after the undefined symbol (area
	          symbols also have an entry in the symbol table as
	          STT_SECTION).  */
      if (option_aof)
	sym->used = (sym->type & SYMBOL_AREA) ? sym->area.info->number : symbolIndex;
      else
	sym->used = symbolIndex + 1;
    }
  return result;
}


void
Symbol_OutputStrings (FILE *outfile, const SymbolOut_t *symOutP)
{
  unsigned count = 0;
  for (unsigned i = 0; i != symOutP->numAllSymbols; ++i)
    {
      const Symbol *sym = symOutP->allSymbolsPP[i];
      if (Area_IsMappingSymbol (sym->str))
	{
	  if (sym->offset >= count)
	    {
	      assert (sym->offset == count);
	      fputc ('$', outfile);
	      fputc (sym->str[1], outfile);
	      fputc ('\0', outfile);
	      count += 3;
	    }
	}
      else if ((sym->type & SYMBOL_AREA) && !option_aof)
	assert (sym->offset == 0);
      else
	{
	  assert (sym->offset == count);
	  count += fwrite (sym->str, 1, sym->len + 1, outfile);
	}
    }
  assert (count == symOutP->stringSize);
}


void
Symbol_OutputForAOF (FILE *outfile, const SymbolOut_t *symOutP)
{
  for (unsigned i = 0; i != symOutP->numAllSymbols; ++i)
    {
      const Symbol *sym = symOutP->allSymbolsPP[i];

      if (sym->type & SYMBOL_AREA)
	{
	  assert (((sym->type & SYMBOL_ABSOLUTE) != 0) == ((sym->area.info->type & AREA_ABS) != 0));
	  const AofSymbol asym =
	    {
	      .Name = armword (sym->offset + 4), /* + 4 to skip the initial length */
	      .Type = armword (SYMBOL_LOCAL | (sym->type & SYMBOL_ABSOLUTE)),
	      .Value = armword ((sym->area.info->type & AREA_ABS) ? Area_GetBaseAddress (sym) : 0),
	      .AreaName = armword (sym->offset + 4) /* + 4 to skip the initial length */
	    };
	  fwrite (&asym, sizeof (AofSymbol), 1, outfile);
	}
      else
	{
	  AofSymbol asym;
	  if (sym->type & SYMBOL_DEFINED)
	    {
	      /* SYMBOL_LOCAL, SYMBOL_GLOBAL */
	      const Value *value;
	      if (sym->value.Tag == ValueCode)
		{
		  codeInit ();
		  value = codeEvalLow (ValueAll, sym->value.Data.Code.len,
				       sym->value.Data.Code.c, NULL);
		}
	      else
		value = &sym->value;

	      /* We can only have Int, Bool and Symbol here.
		 Also Addr is possible for an area mapping symbol when base
		 register is specified.  */
	      int v;
	      switch (value->Tag)
		{
		  case ValueAddr:
		    v = value->Data.Addr.i;
		    break;

		  case ValueInt:
		    v = value->Data.Int.i;
		    break;

		  case ValueBool:
		    v = value->Data.Bool.b;
		    break;

                  case ValueSymbol:
		    v = value->Data.Symbol.offset;
		    break;

		  default:
		    assert (0 && "Wrong value tag selection");
		    v = 0;
		    break;
		}
	      asym.Type = armword (sym->type & SYMBOL_SUPPORTEDBITS);
	      asym.Value = armword (v);
	      /* When it is a non-absolute symbol, we need to specify the
	         area name to which this symbol is relative to.  */
	      asym.AreaName = armword ((sym->type & SYMBOL_ABSOLUTE) ? 0 : sym->area.rel->offset + 4);
	    }
	  else
	    {
	      asym.Type = armword ((sym->type | TYPE_REFERENCE) & SYMBOL_SUPPORTEDBITS);
	      asym.Value = armword ((sym->type & SYMBOL_COMMON) ? sym->value.Data.Int.i : 0);
	      asym.AreaName = armword (0);
	    }
	  asym.Name = armword (sym->offset + 4); /* + 4 to skip the initial length */
	  fwrite (&asym, sizeof (AofSymbol), 1, outfile);
	}
    }
}


#ifndef NO_ELF_SUPPORT
void
Symbol_OutputForELF (FILE *outfile, const SymbolOut_t *symOutP)
{
  /* Output the undefined symbol.  */
  Elf32_Sym asym =
    {
      .st_name = 0,
      .st_value = 0,
      .st_size = 0,
      .st_info = 0,
      .st_other = 0,
      .st_shndx = 0
    };
  fwrite (&asym, sizeof (Elf32_Sym), 1, outfile);

  for (unsigned i = 0; i != symOutP->numAllSymbols; ++i)
    {
      const Symbol *sym = symOutP->allSymbolsPP[i];

      if (sym->type & SYMBOL_AREA)
	{
	  assert (((sym->type & SYMBOL_ABSOLUTE) != 0) == ((sym->area.info->type & AREA_ABS) != 0));
	  assert (SYMBOL_KIND (sym->type) == 0);
          asym.st_name = 0;
	  asym.st_value = (sym->area.info->type & AREA_ABS) ? Area_GetBaseAddress (sym) : 0;
	  asym.st_info = ELF32_ST_INFO (STB_LOCAL, STT_SECTION);
	  asym.st_shndx = sym->area.info->number;
	  assert (asym.st_shndx >= 3);
	  fwrite (&asym, sizeof (Elf32_Sym), 1, outfile);
	}
      else
	{
	  asym.st_name = sym->offset + 1; /* + 1 to skip the initial & extra NUL */
	  if (sym->type & SYMBOL_DEFINED)
	    {
	      /* SYMBOL_LOCAL, SYMBOL_GLOBAL */
	      const Value *value;
	      if (sym->value.Tag == ValueCode)
		{
		  codeInit ();
		  value = codeEvalLow (ValueAll, sym->value.Data.Code.len,
				       sym->value.Data.Code.c, NULL);
		}
	      else
		value = &sym->value;

	      /* We can only have Int, Bool and Symbol here.
		 Also Addr is possible for an area mapping symbol when base
		 register is specified.  */
	      int v;
	      switch (value->Tag)
		{
		  case ValueAddr:
		    v = value->Data.Addr.i;
		    break;

		  case ValueInt:
		    v = value->Data.Int.i;
		    break;

		  case ValueBool:
		    v = value->Data.Bool.b;
		    break;

                  case ValueSymbol:
		    v = value->Data.Symbol.offset;
		    break;

		  default:
		    assert (0 && "Wrong value tag selection");
		    v = 0;
		    break;
		}
	      asym.st_value = v;
	      asym.st_shndx = (sym->type & SYMBOL_ABSOLUTE) && !Area_IsMappingSymbol (sym->str) ? SHN_ABS : sym->areaDef->area.info->number;
	    }
	  else
	    {
	      asym.st_value = 0;
	      asym.st_shndx = SHN_UNDEF;
	    }
	  /* FIXME: support for SYMBOL_COMMON ? */

	  int bind;
	  switch (SYMBOL_KIND (sym->type))
	    {
	      case SYMBOL_LOCAL:
		bind = STB_LOCAL;
		break;
	      case SYMBOL_REFERENCE:
		bind = (sym->type & TYPE_WEAK) ? STB_WEAK : STB_GLOBAL;
		break;
	      case SYMBOL_GLOBAL:
		bind = STB_GLOBAL;
		break;
	      default:
		assert (0);
		break;
	    }
	  asym.st_info = ELF32_ST_INFO (bind, STT_NOTYPE);
	  if (asym.st_shndx == (Elf32_Half)-1)
	    errorAbort ("Internal symbolSymbolELFOutput: unable to find section id for symbol %s", sym->str);
	  else
	    fwrite (&asym, sizeof (Elf32_Sym), 1, outfile);
	}
    }
}
#endif


void
Symbol_FreeSymbolOut (SymbolOut_t *symOutP)
{
  free ((void *)symOutP->allSymbolsPP);
  symOutP->allSymbolsPP = NULL;
}


/**
 * \return NULL when no symbol could be read (and *no* error is given),
 * non-NULL otherwise (even when symbol is not yet known).
 */
static Symbol *
symFlag (unsigned int flags, const char *err)
{
  const Lex lex = lexGetIdNoError ();
  if (lex.tag != LexId)
    return NULL;

  /* When the symbol is not known yet, it will automatically be created.  */
  Symbol *sym = symbolGet (&lex);
  if (Local_IsLocalLabel (sym->str))
    error (ErrorError, "Local labels cannot be %s", err);
  else if (Area_IsMappingSymbol (sym->str))
    error (ErrorError, "Mapping symbols cannot be %s", err);
  else if (sym->type & SYMBOL_RW)
    error (ErrorError, "%s symbols cannot be %s",
           sym->type & SYMBOL_MACRO_LOCAL ? "Local" : "Global", err);
  else
    sym->type |= flags;
  return sym;
}

/**
 * Implements EXPORT / GLOBAL.
 * "EXPORT <symbol>[FPREGARGS,DATA,LEAF,WEAK]"
 * "EXPORT [WEAK]"
 *
 */
bool
c_export (void)
{
  /* 'EXPORT'/'GLOBAL' is not supported when in AAsm compatibility mode.  */
  if (option_abs)
    return true;

  Symbol *sym = symFlag (SYMBOL_REFERENCE, "exported");
  skipblanks ();
  if (Input_Match ('[', true))
    {
      do
	{
	  Lex attribute = lexGetId ();
	  if (sym != NULL
	      && attribute.Data.Id.len == sizeof ("FPREGARGS")-1
	      && !memcmp ("FPREGARGS", attribute.Data.Id.str, attribute.Data.Id.len))
	    sym->type |= SYMBOL_FPREGARGS;
	  else if (sym != NULL
		   && attribute.Data.Id.len == sizeof ("DATA")-1
		   && !memcmp ("DATA", attribute.Data.Id.str, attribute.Data.Id.len))
	    sym->type |= SYMBOL_DATUM;
	  else if (sym != NULL
		   && attribute.Data.Id.len == sizeof ("LEAF")-1
		   && !memcmp ("LEAF", attribute.Data.Id.str, attribute.Data.Id.len))
	    sym->type |= SYMBOL_LEAF;
	  else if (attribute.Data.Id.len == sizeof ("WEAK")-1
		   && !memcmp ("WEAK", attribute.Data.Id.str, attribute.Data.Id.len))
	    {
	      if (sym != NULL)
		sym->type |= SYMBOL_WEAK;
	      else
		oAllExportSymbolsAreWeak = true;
	    }
	  else
	    error (ErrorError, "Illegal EXPORT attribute %s", attribute.Data.Id.str);
	  skipblanks ();
	} while (Input_Match (',', true));
      if (!Input_Match (']', false))
        error (ErrorError, "Missing ]");
    }
  else if (sym == NULL)
    error (ErrorError, "Missing symbol to export");
  return false;
}

/**
 * Implements STRONG.
 */
bool
c_strong (void)
{
  /* 'STRONG' is not supported when in AAsm compatibility mode.  */
  if (option_abs)
    return true;

  if (symFlag (SYMBOL_STRONG, "marked as 'strong'") == NULL)
    error (ErrorError, "Missing symbol to mark as 'strong'");
  return false;
}

/**
 * Implements KEEP.
 *   KEEP <symbol> : enforces <symbol> to be in the output.
 *   KEEP : enforces all symbols to be in the output.
 */
bool
c_keep (void)
{
  /* 'KEEP' is not supported when in AAsm compatibility mode.  */
  if (option_abs)
    return true;

  if (symFlag (SYMBOL_KEEP, "marked to 'keep'") == NULL)
    oKeepAllSymbols = true;
  return false;
}

/**
 * Implements IMPORT / EXTERN.
 *   IMPORT <symbol>[,NOCASE][,WEAK][,COMMON=<size>][,FPREGARGS]
 */
bool
c_import (void)
{
  /* 'IMPORT'/'EXTERN' is not supported when in AAsm compatibility mode.  */
  if (option_abs)
    return true;

  Symbol *sym = symFlag (SYMBOL_REFERENCE, "imported");
  if (sym == NULL)
    {
      error (ErrorError, "Missing symbol for import");
      return false;
    }

  while (Input_Match (',', false))
    {
      Lex attribute = lexGetId ();
      if (attribute.Data.Id.len == sizeof ("NOCASE")-1
          && !memcmp ("NOCASE", attribute.Data.Id.str, attribute.Data.Id.len))
	sym->type |= SYMBOL_NOCASE;
      else if (attribute.Data.Id.len == sizeof ("WEAK")-1
               && !memcmp ("WEAK", attribute.Data.Id.str, attribute.Data.Id.len))
	sym->type |= SYMBOL_WEAK;
      else if (attribute.Data.Id.len == sizeof ("COMMON")-1
               && !memcmp ("COMMON", attribute.Data.Id.str, attribute.Data.Id.len))
        {
	  skipblanks ();
	  if (Input_Match ('=', false))
	    error (ErrorError, "COMMON attribute needs size specification");
	  else
	    {
	      const Value *size = exprBuildAndEval (ValueInt);
	      switch (size->Tag)
	        {
		  case ValueInt:
		    Value_Assign (&sym->value, size);
		    sym->type |= SYMBOL_COMMON;
		    break;
		  default:
		    error (ErrorError, "Illegal COMMON attribute expression");
		    break;
	        }
	    }
	}
      else if (attribute.Data.Id.len == sizeof ("FPREGARGS")-1
               && !memcmp ("FPREGARGS", attribute.Data.Id.str, attribute.Data.Id.len))
	sym->type |= SYMBOL_FPREGARGS;
      else
	error (ErrorError, "Illegal IMPORT attribute %s", attribute.Data.Id.str);
      skipblanks ();
    }
  return false;
}

#ifdef DEBUG
void
symbolPrint (const Symbol *sym)
{
  static const char *symkind[4] = { "UNKNOWN", "LOCAL", "REFERENCE", "GLOBAL" };
  printf ("\"%s\": %s /",
          sym->str, symkind[SYMBOL_KIND (sym->type)]);
  assert (strlen (sym->str) == (size_t)sym->len);
  /* The Symbol::area.info (or Symbol::area.rel) is non-NULL iff the symbol is
     an area name symbol or we have a relative symbol.  */
  assert (!(sym->type & SYMBOL_DEFINED) || ((sym->type & SYMBOL_AREA) || !(sym->type & SYMBOL_ABSOLUTE)) == (sym->area.info != NULL));

  /* Dump the symbol attributes:  */
  if (!(sym->type & SYMBOL_AREA))
    {
      if (sym->type & SYMBOL_ABSOLUTE)
	printf ("absolute/");
      else if (sym->type & SYMBOL_DEFINED)
	printf ("relative to %s/", sym->area.rel ? sym->area.rel->str : "NULL ???");
    }
  if (sym->type & SYMBOL_NOCASE)
    printf ("caseinsensitive/");
  if (sym->type & SYMBOL_WEAK)
    printf ("weak/");
  if (sym->type & SYMBOL_STRONG)
    printf ("strong/");
  if (sym->type & SYMBOL_COMMON)
    printf ("common/");
  if (sym->type & SYMBOL_DATUM)
    printf ("datum/");
  if (sym->type & SYMBOL_FPREGARGS)
    printf ("fp args in regs/");
  if (sym->type & SYMBOL_LEAF)
    printf ("leaf/");
  if (sym->type & SYMBOL_THUMB)
    printf ("thumb/");
  /* Internal attributes: */
  printf (" * /");
  if (sym->type & SYMBOL_MACRO_LOCAL)
    printf ("local macro/");
  if (sym->type & SYMBOL_RW)
    printf ("rw/");
  if (sym->type & SYMBOL_KEEP)
    printf ("keep/");
  if (sym->type & SYMBOL_AREA)
    printf ("area %p/", (void *)sym->area.info);

  printf (", def area \"%s\", size %" PRIu32 ", offset 0x%x, used %d : ",
          sym->areaDef ? sym->areaDef->str : "<NULL>",
          sym->codeSize, sym->offset, sym->used);
  valuePrint (&sym->value);
}

/**
 * Lists all symbols collected so far together with all its attributes.
 */
void
symbolPrintAll (void)
{
  for (int i = 0; i != SYMBOL_TABLESIZE; i++)
    {
      for (const Symbol *sym = symbolTable[i]; sym; sym = sym->next)
	{
	  symbolPrint (sym);
	  printf ("\n");
	}
    }
}
#endif
