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
#include "global.h"
#include "help_lex.h"
#include "local.h"
#include "main.h"
#include "output.h"
#include "symbol.h"

/* For AOF, we output a symbol when it is be exported (or forced exported)
   and it is defined, or imported and referenced in the code.  */
int (SYMBOL_AOF_OUTPUT) (const Symbol *);	/* typedef it */
#define SYMBOL_AOF_OUTPUT(sym) \
  (((sym)->type & (SYMBOL_EXPORT | SYMBOL_KEEP)) \
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
  result->area.ptr /* = result->area.info */ = NULL;
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
  } predefines[] = {
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
    { "c15", 3, 15, SYMBOL_COPREG },
    /* Terminator */
    { NULL, 0, 0, 0 }
  };

  for (int i = 0; predefines[i].name != NULL; i++)
    {
      const Lex l = lexTempLabel (predefines[i].name, predefines[i].len);

      Symbol *s = symbolAdd (&l);
      s->type |= SYMBOL_ABSOLUTE | SYMBOL_DECLARED | predefines[i].type;
      s->value.Tag = ValueInt;
      s->value.Data.Int.i = predefines[i].value;
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
	  if ((search->type & SYMBOL_DEFINED) && !SYMBOL_GETREG (search->type))
	    error (ErrorError, "Redefinition of %.*s",
		   (int)l->Data.Id.len, l->Data.Id.str);
	  else
	    {
	      if (search->type & SYMBOL_AREA)
	        {
	          if (areaCurrentSymbol->value.Data.Int.i != 0)
		    error (ErrorError, "Symbol %.*s is already defined as area with incompatible definition",
		           (int)l->Data.Id.len, l->Data.Id.str);
		}
	      else
		{
		  search->type |= SYMBOL_DEFINED;
		  return search;
		}
	    }
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

  Symbol **isearch;
  for (isearch = &symbolTable[l->Data.Id.hash]; *isearch; isearch = &(*isearch)->next)
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
		  /* Make it a reference symbol.  */
		  sym->type |= SYMBOL_REFERENCE;
		  if (option_pedantic)
		    errorLine (0, NULL, ErrorWarning, "Symbol %s is implicitly imported", sym->str);
		}
	      if (SYMBOL_OUTPUT (sym))
		{
		  if (localTest (sym->str))
		    {
		      void *area; /* FIXME: this is not usefully used. Why ? */
		      int label = -1;
		      int ii;
		      char routine[1024];
		      *routine = 0;
		      if (sscanf (sym->str, localFormat, &area, &label, &ii, &routine) > 2)
			{
			  const char *file;
			  int lineno;
			  localFindRout (routine, &file, &lineno);
			  errorLine (lineno, file, ErrorError, "Missing local label (fwd) with ID %02i in routine '%s'%s", label, *routine ? routine : "<anonymous>", lineno ? " in block starting" : " (unknown location)");
			}
		      else if (sscanf (sym->str + sizeof ("Local$$")-1, "%i$$%s", &ii, routine) > 0)
			{
			  const char *file;
			  int lineno;
			  localFindLocal (ii, &file, &lineno);
			  errorLine (lineno, file, ErrorError, "Missing local label '%s'%s", *routine ? routine : "<anonymous>", lineno ? " in block starting" : " (unknown location)");
			}
		      return 0;
		    }

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
#ifdef DEBUG
  /* symbolPrintAll (); */
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
		  Value value;
		  if (sym->value.Tag == ValueCode)
		    {
		      codeInit ();
		      value = codeEvalLow (ValueAll, sym->value.Data.Code.len,
		                           sym->value.Data.Code.c);
		    }
		  else
		    value = sym->value;

		  int v;
		  switch (value.Tag)
		    {
		      case ValueIllegal:
			errorLine (0, NULL, ErrorError,
			           "Symbol %s cannot be evaluated", sym->str);
			v = 0;
			break;
		      case ValueInt:
		      case ValueAddr:	/* nasty hack */
			v = value.Data.Int.i;
			break;
		      case ValueFloat:
			errorLine (0, NULL, ErrorError,
			           "Linker does not understand float constants (%s)", sym->str);
			v = (int) value.Data.Float.f;
			break;
		      case ValueString:
			v = lexChar2Int (false, value.Data.String.len, value.Data.String.s);
			break;
		      case ValueBool:
			v = value.Data.Bool.b;
			break;
		      case ValueCode:
			errorLine (0, NULL, ErrorError,
			           "Linker does not understand code constants (%s)", sym->str);
			v = 0;
			break;
		      case ValueLateLabel:
			if (!value.Data.Late.late->next	/* Only one late label */
			    && value.Data.Late.late->factor == 1	/* ... occuring one time */
			    && (value.Data.Late.late->symbol->type & SYMBOL_AREA))
			  {		/* ... and it is an area */
			    if (sym->type & SYMBOL_ABSOLUTE)
			      {	/* Change absolute to relative */
				sym->type &= ~SYMBOL_ABSOLUTE;
				v = value.Data.Late.i;
				sym->area.ptr = value.Data.Late.late->symbol;
			      }
			    else if (sym->area.ptr != value.Data.Late.late->symbol)
			      {
				errorLine (0, NULL, ErrorError,
					   "Linker cannot have 2 areas for the same symbol (%s)", sym->str);
			        v = 0;
			      }
			    else
			      v = 0;
			  }
			else
			  {
			    errorLine (0, NULL, ErrorError,
				       "Linker cannot have many late labels for the same symbol (%s)", sym->str);
			    v = 0;
			  }
			break;
		      default:
			errorAbortLine (0, NULL,
			                "Internal symbolSymbolAOFOutput: not possible (%s) (0x%x)", sym->str, value.Tag);
			v = 0;
			break;
		    }
		  asym.Value = v;
		  if ((asym.Type = sym->type) & SYMBOL_ABSOLUTE)
		    asym.AreaName = 0;
		  else
		    asym.AreaName = sym->area.ptr->offset + 4;
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
		  Value value;
		  if (sym->value.Tag == ValueCode)
		    {
		      codeInit ();
		      value = codeEvalLow (ValueAll, sym->value.Data.Code.len, sym->value.Data.Code.c);
		      type = STT_NOTYPE; /* No information to base type on */
		    }
		  else
		    value = sym->value;
		  int v;
		  switch (value.Tag)
		    {
		      case ValueIllegal:
			errorLine (0, NULL, ErrorError, "Symbol %s cannot be evaluated", sym->str);
			v = 0;
			break;
		      case ValueInt:
		      case ValueAddr: /* nasty hack */
			v = value.Data.Int.i;
			break;
		      case ValueFloat:
			errorLine (0, NULL, ErrorError, "Linker does not understand float constants (%s)", sym->str);
			v = (int) value.Data.Float.f;
			break;
		      case ValueString:
			v = lexChar2Int (false, value.Data.String.len, value.Data.String.s);
			break;
		      case ValueBool:
			v = value.Data.Bool.b;
			break;
		      case ValueCode:
			errorLine (0, NULL, ErrorError, "Linker does not understand code constants (%s)", sym->str);
			v = 0;
			break;
		      case ValueLateLabel:
			if (!value.Data.Late.late->next /* Only one late label */
			    && value.Data.Late.late->factor == 1 /* ... occuring one time */
			    && (value.Data.Late.late->symbol->type & SYMBOL_AREA))
			  { /* ... and it is an area */
			    if (sym->type & SYMBOL_ABSOLUTE)
			      { /* Change absolute to relative */
				sym->type &= ~SYMBOL_ABSOLUTE;
				v = value.Data.Late.i;
				sym->area.ptr = value.Data.Late.late->symbol;
			      }
			    else if (sym->area.ptr != value.Data.Late.late->symbol)
			      {
			        errorLine (0, NULL, ErrorError, "Linker cannot have 2 areas for the same symbol (%s)", sym->str);
			        v = 0;
			      }
			    else
			      v = 0;
			  }
			else
			  {
			    errorLine (0, NULL, ErrorError, "Linker cannot have many late labels for the same symbol (%s)", sym->str);
			    v = 0;
			  }
			break;
		      default:
			errorAbortLine (0, NULL, "Internal symbolELFSymbolOutput: not possible (%s) (0x%x)", sym->str, value.Tag);
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

	      switch (SYMBOL_KIND(sym->type))
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

#ifdef DEBUG
void
symbolPrint (const Symbol *sym)
{
  static const char *symkind[4] = { "UNKNOWN", "LOCAL", "REFERENCE", "GLOBAL" };
  printf ("\"%.*s\": %s /",
	  (int)sym->len, sym->str, symkind[SYMBOL_KIND(sym->type)]);
  assert (strlen (sym->str) == (size_t)sym->len);
  /* Dump the symbol attributes:  */
  if (sym->type & SYMBOL_ABSOLUTE)
    printf ("absolute/");
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
    printf ("area/");
  if (sym->type & SYMBOL_NOTRESOLVED)
    printf ("not resolved/");
  switch (SYMBOL_GETREG (sym->type))
    {
      case 0: /* No register, nor coprocessor number.  */
	break;
      case SYMBOL_CPUREG:
	printf ("cpu reg/");
	break;
      case SYMBOL_FPUREG:
	printf ("fpu reg/");
	break;
      case SYMBOL_COPREG:
	printf ("coproc reg/");
	break;
      case SYMBOL_COPNUM:
	printf ("coproc num/");
	break;
      default:
	printf ("??? 0x%x/", SYMBOL_GETREG(sym->type));
	break;
    }
  if (sym->type & SYMBOL_DECLARED)
    printf ("declared/");
  
  printf (" * %p, offset 0x%x, used %d: ",
	  (void *)sym->area.ptr, sym->offset, sym->used);
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
	  if (SYMBOL_GETREG (sym->type))
	    continue;

	  symbolPrint (sym);
	}
    }
}
#endif
