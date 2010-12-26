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
 * symbol.c
 */

#include "config.h"

#include <assert.h>
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

/* For AOF, we output a symbol when it is be exported (or forced exported)
   and it is defined, or imported and referenced in the code for relocation.  */
int (SYMBOL_AOF_OUTPUT) (const Symbol *);	/* typedef it */
#define SYMBOL_AOF_OUTPUT(sym) \
  ((oKeepAllSymbols || ((sym)->type & (SYMBOL_EXPORT | SYMBOL_KEEP))) \
   && (((sym)->type & SYMBOL_DEFINED) || (sym)->used > -1))

/* For ELF, we output all used & defined or referenced symbols (except register
   or coprocessor names).  */
int (SYMBOL_ELF_OUTPUT) (const Symbol *);	/* typedef it */
#define SYMBOL_ELF_OUTPUT(sym) \
  (SYMBOL_AOF_OUTPUT(sym) || (sym)->value.Tag == ValueCode)

#ifndef NO_ELF_SUPPORT
#define SYMBOL_OUTPUT(sym) \
  (option_aof ? SYMBOL_AOF_OUTPUT(sym) : SYMBOL_ELF_OUTPUT(sym))
#else
#define SYMBOL_OUTPUT(sym) \
  SYMBOL_AOF_OUTPUT(sym)
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

void
symbolInit (void)
{
  static const struct {
    const char *name;
    size_t len;
    int value;
    int type;
  } preDefSymbols[] = {
    /* Normal registers */
    { "r0",  2, 0, SYMBOL_CPUREG },  { "R0",  2, 0, SYMBOL_CPUREG },
    { "r1",  2, 1, SYMBOL_CPUREG },  { "R1",  2, 1, SYMBOL_CPUREG },
    { "r2",  2, 2, SYMBOL_CPUREG },  { "R2",  2, 2, SYMBOL_CPUREG },
    { "r3",  2, 3, SYMBOL_CPUREG },  { "R3",  2, 3, SYMBOL_CPUREG },
    { "r4",  2, 4, SYMBOL_CPUREG },  { "R4",  2, 4, SYMBOL_CPUREG },
    { "r5",  2, 5, SYMBOL_CPUREG },  { "R5",  2, 5, SYMBOL_CPUREG },
    { "r6",  2, 6, SYMBOL_CPUREG },  { "R6",  2, 6, SYMBOL_CPUREG },
    { "r7",  2, 7, SYMBOL_CPUREG },  { "R7",  2, 7, SYMBOL_CPUREG },
    { "r8",  2, 8, SYMBOL_CPUREG },  { "R8",  2, 8, SYMBOL_CPUREG },
    { "r9",  2, 9, SYMBOL_CPUREG },  { "R9",  2, 9, SYMBOL_CPUREG },
    { "r10", 3, 10, SYMBOL_CPUREG }, { "R10", 3, 10, SYMBOL_CPUREG },
    { "r11", 3, 11, SYMBOL_CPUREG }, { "R11", 3, 11, SYMBOL_CPUREG },
    { "r12", 3, 12, SYMBOL_CPUREG }, { "R12", 3, 12, SYMBOL_CPUREG },
    { "r13", 3, 13, SYMBOL_CPUREG }, { "R13", 3, 13, SYMBOL_CPUREG },
    { "r14", 3, 14, SYMBOL_CPUREG }, { "R14", 3, 14, SYMBOL_CPUREG },
    { "r15", 3, 15, SYMBOL_CPUREG }, { "R15", 3, 15, SYMBOL_CPUREG },
    /* APCS names */
    { "a1", 2, 0, SYMBOL_CPUREG },  { "A1", 2, 0, SYMBOL_CPUREG },
    { "a2", 2, 1, SYMBOL_CPUREG },  { "A2", 2, 1, SYMBOL_CPUREG },
    { "a3", 2, 2, SYMBOL_CPUREG },  { "A3", 2, 2, SYMBOL_CPUREG },
    { "a4", 2, 3, SYMBOL_CPUREG },  { "A4", 2, 3, SYMBOL_CPUREG },
    { "v1", 2, 4, SYMBOL_CPUREG },  { "V1", 2, 4, SYMBOL_CPUREG },
    { "v2", 2, 5, SYMBOL_CPUREG },  { "V2", 2, 5, SYMBOL_CPUREG },
    { "v3", 2, 6, SYMBOL_CPUREG },  { "V3", 2, 6, SYMBOL_CPUREG },
    { "v4", 2, 7, SYMBOL_CPUREG },  { "V4", 2, 7, SYMBOL_CPUREG },
    { "v5", 2, 8, SYMBOL_CPUREG },  { "V5", 2, 8, SYMBOL_CPUREG },
    { "v6", 2, 9, SYMBOL_CPUREG },  { "V6", 2, 9, SYMBOL_CPUREG },
    { "sb", 2, 9, SYMBOL_CPUREG },  { "SB", 2, 9, SYMBOL_CPUREG },
    { "sl", 2, 10, SYMBOL_CPUREG }, { "SL", 2, 10, SYMBOL_CPUREG },
    { "fp", 2, 11, SYMBOL_CPUREG }, { "FP", 2, 11, SYMBOL_CPUREG },
    { "ip", 2, 12, SYMBOL_CPUREG }, { "IP", 2, 12, SYMBOL_CPUREG },
    { "sp", 2, 13, SYMBOL_CPUREG }, { "SP", 2, 13, SYMBOL_CPUREG },
    { "lr", 2, 14, SYMBOL_CPUREG }, { "LR", 2, 14, SYMBOL_CPUREG },
    { "pc", 2, 15, SYMBOL_CPUREG }, { "PC", 2, 15, SYMBOL_CPUREG },
    /* Backwards compatibility */
    { "rfp", 3, 9, SYMBOL_CPUREG }, { "RFP", 3, 9, SYMBOL_CPUREG },
    /* FPU registers */
    { "f0", 2, 0, SYMBOL_FPUREG }, { "F0", 2, 0, SYMBOL_FPUREG },
    { "f1", 2, 1, SYMBOL_FPUREG }, { "F1", 2, 1, SYMBOL_FPUREG },
    { "f2", 2, 2, SYMBOL_FPUREG }, { "F2", 2, 2, SYMBOL_FPUREG },
    { "f3", 2, 3, SYMBOL_FPUREG }, { "F3", 2, 3, SYMBOL_FPUREG },
    { "f4", 2, 4, SYMBOL_FPUREG }, { "F4", 2, 4, SYMBOL_FPUREG },
    { "f5", 2, 5, SYMBOL_FPUREG }, { "F5", 2, 5, SYMBOL_FPUREG },
    { "f6", 2, 6, SYMBOL_FPUREG }, { "F6", 2, 6, SYMBOL_FPUREG },
    { "f7", 2, 7, SYMBOL_FPUREG }, { "F7", 2, 7, SYMBOL_FPUREG },
    /* FIXME: define single/double precision VFP registers */
    /* Coprocessor numbers */
    { "p0",  2, 0, SYMBOL_COPNUM },
    { "p1",  2, 1, SYMBOL_COPNUM },
    { "p2",  2, 2, SYMBOL_COPNUM },
    { "p3",  2, 3, SYMBOL_COPNUM },
    { "p4",  2, 4, SYMBOL_COPNUM },
    { "p5",  2, 5, SYMBOL_COPNUM },
    { "p6",  2, 6, SYMBOL_COPNUM },
    { "p7",  2, 7, SYMBOL_COPNUM },
    { "p8",  2, 8, SYMBOL_COPNUM },
    { "p9",  2, 9, SYMBOL_COPNUM },
    { "p10", 3, 10, SYMBOL_COPNUM },
    { "p11", 3, 11, SYMBOL_COPNUM },
    { "p12", 3, 12, SYMBOL_COPNUM },
    { "p13", 3, 13, SYMBOL_COPNUM },
    { "p14", 3, 14, SYMBOL_COPNUM },
    { "p15", 3, 15, SYMBOL_COPNUM },
    /* Coprocessor registers */
    { "c0",  2, 0, SYMBOL_COPREG },
    { "c1",  2, 1, SYMBOL_COPREG },
    { "c2",  2, 2, SYMBOL_COPREG },
    { "c3",  2, 3, SYMBOL_COPREG },
    { "c4",  2, 4, SYMBOL_COPREG },
    { "c5",  2, 5, SYMBOL_COPREG },
    { "c6",  2, 6, SYMBOL_COPREG },
    { "c7",  2, 7, SYMBOL_COPREG },
    { "c8",  2, 8, SYMBOL_COPREG },
    { "c9",  2, 9, SYMBOL_COPREG },
    { "c10", 3, 10, SYMBOL_COPREG },
    { "c11", 3, 11, SYMBOL_COPREG },
    { "c12", 3, 12, SYMBOL_COPREG },
    { "c13", 3, 13, SYMBOL_COPREG },
    { "c14", 3, 14, SYMBOL_COPREG },
    { "c15", 3, 15, SYMBOL_COPREG }
  };

  for (size_t i = 0; i < sizeof (preDefSymbols)/sizeof (preDefSymbols[0]); ++i)
    {
      const Lex l = lexTempLabel (preDefSymbols[i].name, preDefSymbols[i].len);

      Symbol *s = symbolAdd (&l);
      s->type |= SYMBOL_ABSOLUTE | SYMBOL_DECLARED | preDefSymbols[i].type;
      s->value = Value_Int (preDefSymbols[i].value);
    }
}


/**
 * Adds a new symbol definition.  When the symbol was already defined, this is
 * flagged as an error unless it is an area symbol of zero size.
 * \return pointer to Symbol, never NULL.
 */
Symbol *
symbolAdd (const Lex *l)
{
  if (l->tag != LexId)
    errorAbort ("Internal symbolAdd: non-ID");

  Symbol **isearch;
  for (isearch = &symbolTable[l->Data.Id.hash]; *isearch; isearch = &(*isearch)->next)
    {
      Symbol *search = *isearch;
      if (EqSymLex (search, l))
	{
	  if ((search->type & SYMBOL_DEFINED) && !SYMBOL_GETREGTYPE (search->type))
	    error (ErrorError, "Redefinition of '%.*s'",
		   (int)l->Data.Id.len, l->Data.Id.str);
	  else
	    {
	      if (search->type & SYMBOL_AREA)
	        {
	          if (areaCurrentSymbol->value.Data.Int.i != 0)
		    error (ErrorError, "Symbol '%.*s' is already defined as area with incompatible definition",
		           (int)l->Data.Id.len, l->Data.Id.str);
		}
	      else
		search->type |= SYMBOL_DEFINED;
	    }
	  return search;
	}
    }
  *isearch = symbolNew (l->Data.Id.str, l->Data.Id.len);
  (*isearch)->type |= SYMBOL_DEFINED;
  return *isearch;
}


/**
 * \return Always a non-NULL value pointing to symbol representing given Lex
 * object.
 */
Symbol *
symbolGet (const Lex *l)
{
  assert (l->tag == LexId);

  Symbol **isearch = NULL;
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
 * Removes symbol from symbol table.
 */
void
symbolRemove (const Lex *l)
{
  assert (l->tag == LexId);

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


/**
 * Calculates number of symbols which need to be written in the output file
 * together with the total string size needed for all those symbols.
 * \param stringSizeNeeded Pointer to a value which will on return contain the
 * total length of symbol strings.
 * \return number of symbols which need to be written in output file.
 */
int
symbolFix (int *stringSizeNeeded)
{
  int nosym = 0;
  int strsize = 0;		/* Always contains its length */
  for (int i = 0; i < SYMBOL_TABLESIZE; i++)
    {
      for (Symbol *sym = symbolTable[i]; sym; sym = sym->next)
	{
	  if (sym->type & SYMBOL_AREA)
	    {
	      /* All AREA symbols are declared and not defined by nature.  */
	      assert ((sym->type & (SYMBOL_DEFINED | SYMBOL_DECLARED)) == SYMBOL_DECLARED);
	      if (!Area_IsImplicit (sym))
		{
		  sym->offset = strsize;
		  strsize += sym->len + 1;
		  nosym++;
		}
	    }
	  else
	    {
	      if (SYMBOL_KIND (sym->type) == 0)
		{
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
			    errorLine (file, lineno, ErrorError, "In area %s routine %s has missing local label %%f%02i%s",
				       area->str, routine, label, routine);
			  else
			    errorLine (NULL, 0, ErrorError, "In area %s there is a missing local label %%f%02i%s",
				       area->str, label, Local_ROUTIsEmpty (routine) ? "" : routine);
			}
		      sym->type |= SYMBOL_REFERENCE;
		    }
		  else
		    {
		      /* Make it a reference symbol.  */
		      sym->type |= SYMBOL_REFERENCE;
		      errorLine (NULL, 0, ErrorWarning, "Symbol %s is implicitly imported", sym->str);
		    }
		}
	      if (SYMBOL_OUTPUT (sym))
		{
		  sym->offset = strsize;
		  strsize += sym->len + 1;
		  sym->used = nosym++;
		}
	      else
		{
		  sym->type &= ~SYMBOL_REFERENCE;
		  sym->used = -1;
		}
	    }
	}
    }
#ifdef DEBUG_SYMBOL
  symbolPrintAll ();
#endif
  /* printf("Number of symbols selected: %d, size needed %d bytes\n", nosym, strsize); */
  *stringSizeNeeded = strsize;
  return nosym;
}

/**
 * This should exactly write *stringSizeNeeded bytes (with *stringSizeNeeded
 * the value returned by symbolFix()).
 */
void
symbolStringOutput (FILE *outfile)	/* Count already output */
{
  for (int i = 0; i < SYMBOL_TABLESIZE; i++)
    {
      for (const Symbol *sym = symbolTable[i]; sym != NULL; sym = sym->next)
	{
	  if (SYMBOL_OUTPUT (sym)
	      || ((sym->type & SYMBOL_AREA) && !Area_IsImplicit (sym)))
	    fwrite (sym->str, 1, sym->len + 1, outfile);
	}
    }
}

void
symbolSymbolAOFOutput (FILE *outfile)
{
  for (int i = 0; i < SYMBOL_TABLESIZE; i++)
    {
      for (Symbol *sym = symbolTable[i]; sym; sym = sym->next)
	{
	  if (!(sym->type & SYMBOL_AREA) && SYMBOL_AOF_OUTPUT (sym))
	    {
	      AofSymbol asym;
	      asym.Name = sym->offset + 4; /* + 4 to skip the initial length */
	      if (sym->type & SYMBOL_DEFINED)
		{
		  const Value *value;
		  if (sym->value.Tag == ValueCode)
		    {
		      codeInit ();
		      value = codeEvalLow (ValueAll, sym->value.Data.Code.len,
		                           sym->value.Data.Code.c, NULL);
		    }
		  else
		    value = &sym->value;

		  int v;
		  switch (value->Tag)
		    {
		      case ValueInt:
			v = value->Data.Int.i;
			break;

		      case ValueBool:
			v = value->Data.Bool.b;
			break;

		      case ValueCode:
			/* Support <ValueInt> <ValueSymbol> <Op_add> */
			if (value->Data.Code.len == 3
			    && value->Data.Code.c[0].Tag == CodeValue
			    && value->Data.Code.c[0].Data.value.Tag == ValueInt
			    && value->Data.Code.c[1].Tag == CodeValue
			    && value->Data.Code.c[1].Data.value.Tag == ValueSymbol
			    && value->Data.Code.c[1].Data.value.Data.Symbol.factor == 1
			    && value->Data.Code.c[2].Tag == CodeOperator
			    && value->Data.Code.c[2].Data.op == Op_add)
			  {
			    v = value->Data.Code.c[0].Data.value.Data.Int.i;
			    break;
			  }
			/* Fall through.  */

		      default:
			errorLine (NULL, 0, ErrorError,
			           "Symbol %s cannot be evaluated for storage in output format", sym->str);
			v = 0;
			break;
		    }
		  asym.Value = v;
		  /* When it is a non-absolute symbol, we need to specify the
		     area name to which this symbol is relative to.  */
		  if ((asym.Type = sym->type) & SYMBOL_ABSOLUTE)
		    asym.AreaName = 0;
		  else
		    asym.AreaName = sym->area.rel->offset + 4;
		}
	      else
		{
		  asym.Type = sym->type | TYPE_REFERENCE;
		  asym.Value = (sym->type & SYMBOL_COMMON) ? sym->value.Data.Int.i : 0;
		  asym.AreaName = 0;
		}
	      asym.Name     = armword (asym.Name);
	      asym.Type     = armword (asym.Type & SYMBOL_SUPPORTEDBITS);
	      asym.Value    = armword (asym.Value);
	      asym.AreaName = armword (asym.AreaName);
	      fwrite (&asym, sizeof (AofSymbol), 1, outfile);
	    }
	  else if (sym->type & SYMBOL_AREA)
	    {
	      if (!Area_IsImplicit (sym))
		{
	          const AofSymbol asym =
		    {
		      .Name = armword (sym->offset + 4), /* + 4 to skip the initial length */
		      .Type = armword (SYMBOL_KIND(sym->type) | SYMBOL_LOCAL),
		      .Value = armword (0),
		      .AreaName = armword (sym->offset + 4) /* + 4 to skip the initial length */
		    };
		  fwrite (&asym, sizeof (AofSymbol), 1, outfile);
		}
	    }
	}
    }
}

#ifndef NO_ELF_SUPPORT
static int
findAreaIndex (const struct AREA *area)
{
  int i = 3;
  for (const Symbol *ap = areaHeadSymbol; ap != NULL; ap = ap->area.info->next)
    {
      if (Area_IsImplicit (ap))
	continue;

      if (area == (const struct AREA *)ap)
        return i;
      i++;
      if (ap->area.info->norelocs > 0)
        i++; /* We will be outputting a relocation section as well */
    }

  return -1;
}

void
symbolSymbolELFOutput (FILE *outfile)
{
  /* Output the undefined symbol */
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

  for (int i = 0; i < SYMBOL_TABLESIZE; i++)
    {
      for (Symbol *sym = symbolTable[i]; sym; sym = sym-> next)
	{
	  if (!(sym->type & SYMBOL_AREA) && SYMBOL_ELF_OUTPUT (sym))
	    {
	      int type = 0, bind;
	      asym.st_name = sym->offset + 1; /* + 1 to skip the initial & extra NUL */
	      if (sym->type & SYMBOL_DEFINED)
		{
		  const Value *value;
		  if (sym->value.Tag == ValueCode)
		    {
		      codeInit ();
		      value = codeEvalLow (ValueAll, sym->value.Data.Code.len, sym->value.Data.Code.c, NULL);
		      type = STT_NOTYPE; /* No information to base type on */
		    }
		  else
		    value = &sym->value;
		  int v;
		  switch (value->Tag)
		    {
		      case ValueInt:
			v = value->Data.Int.i;
			break;

		      case ValueBool:
			v = value->Data.Bool.b;
			break;

		      case ValueCode:
			/* Support <ValueInt> <ValueSymbol> <Op_add> */
			if (value->Data.Code.len == 3
			    && value->Data.Code.c[0].Tag == CodeValue
			    && value->Data.Code.c[0].Data.value.Tag == ValueInt
			    && value->Data.Code.c[1].Tag == CodeValue
			    && value->Data.Code.c[1].Data.value.Tag == ValueSymbol
			    && value->Data.Code.c[1].Data.value.Data.Symbol.factor == 1
			    && value->Data.Code.c[2].Tag == CodeOperator
			    && value->Data.Code.c[2].Data.op == Op_add)
			  {
			    v = value->Data.Code.c[0].Data.value.Data.Int.i;
			    break;
			  }
			/* Fall through.  */
			
		      default:
			errorLine (NULL, 0, ErrorError,
			           "Symbol %s cannot be evaluated for storage in output format", sym->str);
			v = 0;
			break;
		    }
		  asym.st_value = v;
		  if (sym->type & SYMBOL_ABSOLUTE)
		    asym.st_shndx = 0;
		  else
		    /* Inefficient, needs fixing later */
		    asym.st_shndx = findAreaIndex (sym->area.info);
		}
	      else
		{
		  asym.st_value = 0;
		  asym.st_shndx = 0;
		}

	      switch (SYMBOL_KIND (sym->type))
		{
		  case TYPE_LOCAL:
		    bind = STB_LOCAL;
		    break;
		  case TYPE_REFERENCE:
		    bind = (sym->type & TYPE_WEAK) ? STB_WEAK : STB_GLOBAL;
		    break;
		  case TYPE_GLOBAL:
		    bind = STB_GLOBAL;
		    break;
		  default:
		    bind = 0; /* TODO: give error ? */
		    break;
		}

	      asym.st_info = ELF32_ST_INFO(bind, type);
	      if (asym.st_shndx == 65535)
		errorAbort ("Internal symbolSymbolELFOutput: unable to find section id");
	      else
		fwrite (&asym, sizeof (Elf32_Sym), 1, outfile);
	    }
	  else if (sym->type & SYMBOL_AREA)
	    {
	      if (!Area_IsImplicit (sym))
		{
		  asym.st_info = ELF32_ST_INFO ((sym->type & SYMBOL_GLOBAL) ? STB_GLOBAL : STB_LOCAL, STT_SECTION);
		  asym.st_name = sym->offset + 1; /* + 1 to skip initial & extra NUL */
		  asym.st_value = 0;
		  asym.st_shndx = findAreaIndex ((const struct AREA *)sym);
		  fwrite (&asym, sizeof (Elf32_Sym), 1, outfile);
		}
	    }
	}
    }
}
#endif

/**
 * \return NULL when no symbol could be read, non-NULL otherwise (even when
 * symbol is not yet known).
 */
static Symbol *
symFlag (unsigned int flags, const char *err)
{
  const Lex lex = lexGetId ();
  if (lex.tag != LexId)
    return NULL;

  /* When the symbol is not known yet, it will automatically be created.  */
  Symbol *sym = symbolGet (&lex);
  if (Local_IsLocalLabel (sym->str))
    error (ErrorError, "Local labels cannot be %s", err);
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
  Symbol *sym = symFlag (SYMBOL_REFERENCE | SYMBOL_DECLARED, "exported");
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
 */
bool
c_keep (void)
{
  if (symFlag (SYMBOL_KEEP | SYMBOL_DECLARED, "marked to 'keep'"))
    oKeepAllSymbols = true;
  return false;
}

/**
 * Implements IMPORT / EXTERN.
 */
bool
c_import (void)
{
  Symbol *sym = symFlag (SYMBOL_REFERENCE | SYMBOL_DECLARED, "imported");
  if (sym == NULL)
    return false;

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
  /* FIXME: assert (!(sym->type & SYMBOL_DEFINED) || ((sym->type & SYMBOL_AREA) || !(sym->type & SYMBOL_ABSOLUTE)) == (sym->area.info != NULL)); */

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
  if (sym->type & SYMBOL_KEEP)
    printf ("keep/");
  if (sym->type & SYMBOL_AREA)
    printf ("area %p/", (void *)sym->area.info);
  if (sym->type & SYMBOL_NOTRESOLVED)
    printf ("not resolved/");
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
  if (sym->type & SYMBOL_DECLARED)
    printf ("declared/");
  
  printf (" * offset 0x%x, used %d: ", sym->offset, sym->used);
  valuePrint (&sym->value);
}

/**
   * Lists all symbols collected so far together with all its attributes.
 */
void
symbolPrintAll (void)
{
  for (int i = 0; i < SYMBOL_TABLESIZE; i++)
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
