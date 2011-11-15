/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
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
#elif HAVE_INTTYPES_H
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

typedef struct
{
  const char *name;
  size_t len;
  int value;
  int type; /* SYMBOL_CPUREG, etc.  */
} Symbol_PreDef_t;

/* These symbol registers are always defined.  */
static const Symbol_PreDef_t oSymRegs[] =
{
  { "r0", sizeof ("r0")-1, 0, SYMBOL_CPUREG }, { "R0", sizeof ("R0")-1, 0, SYMBOL_CPUREG },
  { "r1", sizeof ("r1")-1, 1, SYMBOL_CPUREG }, { "R1", sizeof ("R1")-1, 1, SYMBOL_CPUREG },
  { "r2", sizeof ("r2")-1, 2, SYMBOL_CPUREG }, { "R2", sizeof ("R2")-1, 2, SYMBOL_CPUREG },
  { "r3", sizeof ("r3")-1, 3, SYMBOL_CPUREG }, { "R3", sizeof ("R3")-1, 3, SYMBOL_CPUREG },
  { "r4", sizeof ("r4")-1, 4, SYMBOL_CPUREG }, { "R4", sizeof ("R4")-1, 4, SYMBOL_CPUREG },
  { "r5", sizeof ("r5")-1, 5, SYMBOL_CPUREG }, { "R5", sizeof ("R5")-1, 5, SYMBOL_CPUREG },
  { "r6", sizeof ("r6")-1, 6, SYMBOL_CPUREG }, { "R6", sizeof ("R6")-1, 6, SYMBOL_CPUREG },
  { "r7", sizeof ("r7")-1, 7, SYMBOL_CPUREG }, { "R7", sizeof ("R7")-1, 7, SYMBOL_CPUREG },
  { "r8", sizeof ("r8")-1, 8, SYMBOL_CPUREG }, { "R8", sizeof ("R8")-1, 8, SYMBOL_CPUREG },
  { "r9", sizeof ("r9")-1, 9, SYMBOL_CPUREG }, { "R9", sizeof ("R9")-1, 9, SYMBOL_CPUREG },
  { "r10", sizeof ("r10")-1, 10, SYMBOL_CPUREG }, { "R10", sizeof ("R10")-1, 10, SYMBOL_CPUREG },
  { "r11", sizeof ("r11")-1, 11, SYMBOL_CPUREG }, { "R11", sizeof ("R11")-1, 11, SYMBOL_CPUREG },
  { "r12", sizeof ("r12")-1, 12, SYMBOL_CPUREG }, { "R12", sizeof ("R12")-1, 12, SYMBOL_CPUREG },
  { "r13", sizeof ("r13")-1, 13, SYMBOL_CPUREG }, { "R13", sizeof ("R13")-1, 13, SYMBOL_CPUREG },
  { "r14", sizeof ("r14")-1, 14, SYMBOL_CPUREG }, { "R14", sizeof ("R14")-1, 14, SYMBOL_CPUREG },
  { "r15", sizeof ("r15")-1, 15, SYMBOL_CPUREG }, { "R15", sizeof ("R15")-1, 15, SYMBOL_CPUREG },
  { "lr", sizeof ("lr")-1, 14, SYMBOL_CPUREG }, { "LR", sizeof ("LR")-1, 14, SYMBOL_CPUREG },
  { "pc", sizeof ("pc")-1, 15, SYMBOL_CPUREG }, { "PC", sizeof ("PC")-1, 15, SYMBOL_CPUREG },
  /* Coprocessor numbers */
  { "p0", sizeof ("p0")-1, 0, SYMBOL_COPNUM },
  { "p1", sizeof ("p1")-1, 1, SYMBOL_COPNUM },
  { "p2", sizeof ("p2")-1, 2, SYMBOL_COPNUM },
  { "p3", sizeof ("p3")-1, 3, SYMBOL_COPNUM },
  { "p4", sizeof ("p4")-1, 4, SYMBOL_COPNUM },
  { "p5", sizeof ("p5")-1, 5, SYMBOL_COPNUM },
  { "p6", sizeof ("p6")-1, 6, SYMBOL_COPNUM },
  { "p7", sizeof ("p7")-1, 7, SYMBOL_COPNUM },
  { "p8", sizeof ("p8")-1, 8, SYMBOL_COPNUM },
  { "p9", sizeof ("p9")-1, 9, SYMBOL_COPNUM },
  { "p10", sizeof ("p10")-1, 10, SYMBOL_COPNUM },
  { "p11", sizeof ("p11")-1, 11, SYMBOL_COPNUM },
  { "p12", sizeof ("p12")-1, 12, SYMBOL_COPNUM },
  { "p13", sizeof ("p13")-1, 13, SYMBOL_COPNUM },
  { "p14", sizeof ("p14")-1, 14, SYMBOL_COPNUM },
  { "p15", sizeof ("p15")-1, 15, SYMBOL_COPNUM },
  /* Coprocessor registers */
  { "c0", sizeof ("c0")-1, 0, SYMBOL_COPREG },
  { "c1", sizeof ("c1")-1, 1, SYMBOL_COPREG },
  { "c2", sizeof ("c2")-1, 2, SYMBOL_COPREG },
  { "c3", sizeof ("c3")-1, 3, SYMBOL_COPREG },
  { "c4", sizeof ("c4")-1, 4, SYMBOL_COPREG },
  { "c5", sizeof ("c5")-1, 5, SYMBOL_COPREG },
  { "c6", sizeof ("c6")-1, 6, SYMBOL_COPREG },
  { "c7", sizeof ("c7")-1, 7, SYMBOL_COPREG },
  { "c8", sizeof ("c8")-1, 8, SYMBOL_COPREG },
  { "c9", sizeof ("c9")-1, 9, SYMBOL_COPREG },
  { "c10", sizeof ("c10")-1, 10, SYMBOL_COPREG },
  { "c11", sizeof ("c11")-1, 11, SYMBOL_COPREG },
  { "c12", sizeof ("c12")-1, 12, SYMBOL_COPREG },
  { "c13", sizeof ("c13")-1, 13, SYMBOL_COPREG },
  { "c14", sizeof ("c14")-1, 14, SYMBOL_COPREG },
  { "c15", sizeof ("c15")-1, 15, SYMBOL_COPREG }
};

/* These symbol registers are always defined when APCS is selected.  */
static const Symbol_PreDef_t oSymRegsAPCS[] =
{
  { "a1", sizeof ("a1")-1, 0, SYMBOL_CPUREG },
  { "a2", sizeof ("a2")-1, 1, SYMBOL_CPUREG },
  { "a3", sizeof ("a3")-1, 2, SYMBOL_CPUREG },
  { "a4", sizeof ("a4")-1, 3, SYMBOL_CPUREG },
  { "v1", sizeof ("v1")-1, 4, SYMBOL_CPUREG },
  { "v2", sizeof ("v2")-1, 5, SYMBOL_CPUREG },
  { "v3", sizeof ("v3")-1, 6, SYMBOL_CPUREG },
  { "v4", sizeof ("v4")-1, 7, SYMBOL_CPUREG },
  { "v5", sizeof ("v5")-1, 8, SYMBOL_CPUREG },
  { "ip", sizeof ("ip")-1, 12, SYMBOL_CPUREG },
  { "sp", sizeof ("sp")-1, 13, SYMBOL_CPUREG },
};

/* These symbol registers are defined whe FPA is selected.  */
static const Symbol_PreDef_t oSymRegsFPA[] =
{
  { "f0", sizeof ("f0")-1, 0, SYMBOL_FPUREG }, { "F0", sizeof ("F0")-1, 0, SYMBOL_FPUREG },
  { "f1", sizeof ("f1")-1, 1, SYMBOL_FPUREG }, { "F1", sizeof ("F1")-1, 1, SYMBOL_FPUREG },
  { "f2", sizeof ("f2")-1, 2, SYMBOL_FPUREG }, { "F2", sizeof ("F2")-1, 2, SYMBOL_FPUREG },
  { "f3", sizeof ("f3")-1, 3, SYMBOL_FPUREG }, { "F3", sizeof ("F3")-1, 3, SYMBOL_FPUREG },
  { "f4", sizeof ("f4")-1, 4, SYMBOL_FPUREG }, { "F4", sizeof ("F4")-1, 4, SYMBOL_FPUREG },
  { "f5", sizeof ("f5")-1, 5, SYMBOL_FPUREG }, { "F5", sizeof ("F5")-1, 5, SYMBOL_FPUREG },
  { "f6", sizeof ("f6")-1, 6, SYMBOL_FPUREG }, { "F6", sizeof ("F6")-1, 6, SYMBOL_FPUREG },
  { "f7", sizeof ("f7")-1, 7, SYMBOL_FPUREG }, { "F7", sizeof ("F7")-1, 7, SYMBOL_FPUREG },
};

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
symbolFree (Symbol *symP)
{
  valueFree (&symP->value);
  free (symP);
}

static bool
EqSymLex (const Symbol *str, const Lex *lx)
{
  if (str->len != lx->Data.Id.len)
    return false;
  return !memcmp (str->str, lx->Data.Id.str, str->len);
}

static void
Symbol_PreDefReg (const char *regname, size_t namelen, int value, int type)
{
  const Lex l = lexTempLabel (regname, namelen);
  Symbol *s = symbolGet (&l);
  s->type |= SYMBOL_DEFINED | SYMBOL_ABSOLUTE | type;
  s->value = Value_Int (value);
}

void
Symbol_Init (void)
{
  for (size_t i = 0; i != sizeof (oSymRegs)/sizeof (oSymRegs[0]); ++i)
    Symbol_PreDefReg (oSymRegs[i].name, oSymRegs[i].len, oSymRegs[i].value, oSymRegs[i].type);

  if (gIsAPCS)
    {
      for (size_t i = 0; i != sizeof (oSymRegsAPCS)/sizeof (oSymRegsAPCS[0]); ++i)
	Symbol_PreDefReg (oSymRegsAPCS[i].name, oSymRegsAPCS[i].len, oSymRegsAPCS[i].value, oSymRegsAPCS[i].type);
      Symbol_PreDefReg ((gOptionAPCS & APCS_OPT_REENTRANT) ? "sb" : "v6",
                        sizeof ("sb")-1, 9, SYMBOL_CPUREG);
      Symbol_PreDefReg ((gOptionAPCS & APCS_OPT_SWSTACKCHECK) ? "sl" : "v7",
                        sizeof ("sl")-1, 10, SYMBOL_CPUREG);
      Symbol_PreDefReg ((gOptionAPCS & APCS_OPT_FRAMEPTR) ? "fp" : "v8",
                        sizeof ("fp")-1, 11, SYMBOL_CPUREG);
    }

  if (1 /* FIXME: only when FPA is selected.  */)
    {
      for (size_t i = 0; i != sizeof (oSymRegsFPA)/sizeof (oSymRegsFPA[0]); ++i)
	Symbol_PreDefReg (oSymRegsFPA[i].name, oSymRegsFPA[i].len, oSymRegsFPA[i].value, oSymRegsFPA[i].type);
    }

  /* FIXME: define single/double precision VFP registers */
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
      if (SYMBOL_GETREGTYPE(symbol->type) != SYMBOL_GETREGTYPE(newSymbolType))
	{
	  error (ErrorError, "Label %s is already defined as a different register type", symbol->str);
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
	      error (ErrorError, "Label %s can not be redefined with a different value", symbol->str);
	      return true;
	    }
	}
    }
  symbol->type |= newSymbolType | SYMBOL_DEFINED;
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
	  Symbol *toFreeP = *isearch;
	  *isearch = toFreeP->next;
	  symbolFree (toFreeP);
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

  bool doOutput = (oKeepAllSymbols || (sym->type & SYMBOL_KEEP) || ((sym->type & SYMBOL_EXPORT) && sym->used >= 0))
		    && !SYMBOL_GETREGTYPE (sym->type)
		    && !Local_IsLocalLabel (sym->str);
  return doOutput;
}


static int
SymbolCompare (const void *symPP1, const void *symPP2)
{
  const Symbol *symP1 = *(const Symbol **)symPP1;
  const Symbol *symP2 = *(const Symbol **)symPP2;
  return strcasecmp (symP1->str, symP2->str);
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
	  if (sym->type & SYMBOL_AREA)
	    {
	      /* At this point sym->used is the area (AOF) or section (ELF)
	         number for all non-implicit area's, see start of outputAof()
		 and outputElf().  */
	      assert ((Area_IsImplicit (sym) && sym->used == -1) || (!Area_IsImplicit (sym) && sym->used >= 0));
	      /* All AREA symbols are local ones.  */
	      assert (SYMBOL_KIND (sym->type) == 0);
	    }
	  else
	    {
	      /* At this point sym->used is -1 when it is not needed for
	         relocation, either is 0 when used for relocation.  */
	      assert (sym->used == -1 || sym->used == 0);
	      if (SYMBOL_KIND (sym->type) == 0)
		{
		  /* Make it a reference symbol.  */
		  sym->type |= SYMBOL_REFERENCE;

		  if (Local_IsLocalLabel (sym->str))
		    {
		      Symbol *area;
		      int label = -1;
		      int ii;
		      char routine[1024];
		      *routine = 0;
		      if (sscanf (sym->str, Local_IntLabelFormat, &area, &label, &ii, &routine) > 2)
			{
			  const char *file;
			  int lineno;
			  Local_FindROUT (routine, &file, &lineno);
			  if (!Local_ROUTIsEmpty (routine) && file != NULL)
			    errorLine (file, lineno, ErrorAbort, "In area %s routine %s has missing local label %%F%02i%s",
				       area->str, routine, label, routine);
			  else
			    errorLine (NULL, 0, ErrorAbort, "In area %s there is a missing local label %%F%02i%s",
				       area->str, label, Local_ROUTIsEmpty (routine) ? "" : routine);
			}
		    }
		  else
		    errorLine (NULL, 0, ErrorWarning, "Symbol %s is implicitly imported", sym->str);
		}
	    }
	  if (Area_IsMappingSymbol (sym->str) || NeedToOutputSymbol (sym))
	    {
	      ++result.numAllSymbols;
	      if ((sym->type & SYMBOL_AREA) || SYMBOL_KIND (sym->type) == SYMBOL_LOCAL)
		++result.numLocalSymbols;
	      if (!(sym->type & SYMBOL_AREA))
		sym->used = 0;
	    }
	  else if (!(sym->type & SYMBOL_AREA))
	    sym->used = -1;
	}
    }

  /* Claim space.  */
  if ((result.allSymbolsPP = malloc (result.numAllSymbols * sizeof (Symbol *))) == NULL)
    errorOutOfMem ();

  /* Run over symbols again, and start assigning them in our symbol output
     array.  */
  unsigned localSymbolIndex = 0;
  unsigned globalSymbolIndex = result.numLocalSymbols;
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
  assert (localSymbolIndex == result.numLocalSymbols && globalSymbolIndex == result.numAllSymbols);

  /* Sort local and global symbols individually.  */
  qsort (&result.allSymbolsPP[0], result.numLocalSymbols, sizeof (Symbol *), SymbolCompare);
  qsort (&result.allSymbolsPP[result.numLocalSymbols], result.numAllSymbols - result.numLocalSymbols, sizeof (Symbol *), SymbolCompare);

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
	      result.stringSize += 3;
	    }
	  else
	    sym->offset = mapSymbols[mappingSymbolindex].offset;
	}
      else
	{
	  /* For ELF, section names are not used in the string table.  */
	  if ((sym->type & SYMBOL_AREA) && !option_aof)
	    sym->offset = 0;
	  else
	    {
	      sym->offset = result.stringSize;
	      result.stringSize += sym->len + 1;
	    }
	}
      if (!(sym->type & SYMBOL_AREA))
	sym->used = symbolIndex;
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
	  asym.st_shndx = sym->used;
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
	      asym.st_shndx = (sym->type & SYMBOL_ABSOLUTE) && !Area_IsMappingSymbol (sym->str) ? SHN_ABS : sym->areaDef->used;
	    }
	  else
	    {
	      asym.st_value = 0;
	      asym.st_shndx = SHN_UNDEF;
	    }

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
  if (symFlag (SYMBOL_KEEP, "marked to 'keep'") == NULL)
    oKeepAllSymbols = true;
  return false;
}

/**
 * Implements IMPORT / EXTERN.
 */
bool
c_import (void)
{
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
  printf ("\"%.*s\": %s /",
	  (int)sym->len, sym->str, symkind[SYMBOL_KIND (sym->type)]);
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
  switch (SYMBOL_GETREGTYPE (sym->type))
    {
      case 0: /* No register, nor coprocessor number.  */
	break;
      case SYMBOL_CPUREG:
	printf ("CPU reg/");
	break;
      case SYMBOL_FPUREG:
	printf ("FPU reg/");
	break;
      case SYMBOL_COPREG:
	printf ("coprocessor reg/");
	break;
      case SYMBOL_COPNUM:
	printf ("coprocessor num/");
	break;
      default:
	printf ("??? 0x%x/", SYMBOL_GETREGTYPE (sym->type));
	break;
    }

  printf (", def area %*.s, size %zd, ", (int)sym->areaDef->len, sym->areaDef->str,
          sym->codeSize);
  printf (", offset 0x%x, used %d ", sym->offset, sym->used);
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
	  /* We skip all internally defined register names and coprocessor
	     numbers.  */
	  if (SYMBOL_GETREGTYPE (sym->type))
	    continue;

	  symbolPrint (sym);
	  printf ("\n");
	}
    }
}
#endif
