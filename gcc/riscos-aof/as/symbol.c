
/*
 *   symbol.c
 * Copyright © 1992 Niklas Röjemo
 */
#include "sdk-config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "main.h"
#include "symbol.h"
#include "global.h"
#include "error.h"
#include "aoffile.h"
#include "elf.h"
#include "code.h"
#include "help_lex.h"
#include "local.h"
#include "area.h"
#include "output.h"

#define SYMBOL_OUTPUT(sym) \
  (((sym)->type & (SYMBOL_EXPORT|SYMBOL_KEEP))>0 && \
   (((sym)->type & SYMBOL_DEFINED)>0 || (sym)->used > -1))
  /* must be exported & defined, or imported and referenced */
int (SYMBOL_OUTPUT) (const Symbol *);	/* typedef it */

static Symbol *symbolTable[SYMBOL_TABLESIZE];

static Symbol *
symbolNew (int len, const char *str)
{
  Symbol *result;
  if ((result = (Symbol *) malloc (sizeof (Symbol) + len)) != 0)
    {
      result->next = 0;
      result->type = result->declared = result->offset = 0;
      result->value.Tag.t = ValueIllegal;
      result->value.Tag.v = ValueConst;
      result->used = -1;
      result->len = len;
      memcpy (result->str, str, len);
      result->str[len] = 0;
    }
  else
    {
      printf("%d  %s\n", sizeof (Symbol) + len, str);
      errorOutOfMem ("symbolNew");
    }
  return result;
}

static BOOL
EqSymLex (Symbol * str, Lex * lx)
{
  char *s, *l;
  int i;
  if (str->len != lx->LexId.len)
    return FALSE;
  for (i = str->len, s = str->str, l = lx->LexId.str; i > 0; i--)
    if (*s++ != *l++)
      return FALSE;
  return TRUE;
}


Symbol *
symbolAdd (Lex l)
{
  Symbol **isearch = &symbolTable[l.LexId.hash];
  if (l.tag != LexId)
    error (ErrorSerious, FALSE, "Internal symbolAdd: non-ID");
  while (*isearch)
    {
      if (EqSymLex (*isearch, &l))
	{
	  if ((*isearch)->type & SYMBOL_DEFINED)
	    {
	      strncpy (er, l.LexId.str, l.LexId.len);
	      er[l.LexId.len] = 0;
	      error (ErrorError, TRUE, "Redefinition of %s", er);
	    }
	  else
	    {
	      if ((*isearch)->type == SYMBOL_AREA)
		{
		  strncpy (er, l.LexId.str, l.LexId.len);
		  er[l.LexId.len] = 0;
		  error (ErrorError, TRUE, "Area %s is already defined", er);
		}
	      else
		{
		  (*isearch)->type |= SYMBOL_DEFINED;
		  return *isearch;
		}
	    }
	}
      isearch = &((*isearch)->next);
    }
  *isearch = symbolNew (l.LexId.len, l.LexId.str);
  (*isearch)->type |= SYMBOL_DEFINED;
  return *isearch;
}

Symbol *
symbolGet (Lex l)
{
  Symbol **isearch = NULL;
  if (l.tag != LexId)
    {
      if (l.tag == LexNone)
	{
	  isearch = &symbolTable[0];
	  while (*isearch)
	    isearch = &((*isearch)->next);
	  *isearch = symbolNew (7, "|Dummy|");
	}
      else
	error (ErrorSerious, FALSE, "Internal symbolGet: non-ID");
    }
  else
    {
      isearch = &symbolTable[l.LexId.hash];
      while (*isearch)
	{
	  if (EqSymLex (*isearch, &l))
	    return *isearch;
	  isearch = &((*isearch)->next);
	}
    }
  *isearch = symbolNew (l.LexId.len, l.LexId.str);
  return *isearch;
}

Symbol *
symbolFind (Lex l)
{
  Symbol **isearch;
  if (l.tag != LexId)
    return NULL;
  isearch = &symbolTable[l.LexId.hash];
  while (*isearch)
    {
      if (EqSymLex (*isearch, &l))
	return *isearch;
      isearch = &((*isearch)->next);
    }
  return NULL;
}

static int stringtablesize = -1;

int
symbolFix (void)		/* Returns number of symbols */
{
  int nosym = 0;
  int strsize = 4;		/* Always contains its length */
  int i;
  Symbol *sym;
  char routine[1024], area[1024];
  const char *file;
  long int lineno;

  for (i = 0; i < SYMBOL_TABLESIZE; i++)
    {
      for (sym = symbolTable[i]; sym; sym = sym->next)
	{
	  if (sym->type == SYMBOL_AREA)
	    {
	      sym->offset = strsize;
	      strsize += sym->len + 1;
	    }
	  else
	    {
	      if (SYMBOL_KIND (sym->type) == 0)
		sym->type |= SYMBOL_REFERENCE;
	      if (SYMBOL_OUTPUT (sym) && sym->value.Tag.v == ValueConst)
		/*(sym->used >= 0 || sym->type & SYMBOL_REFERENCE) */
		{
		  signed int label = -1, i;
		  if (localTest (sym->str))
		    {
		      *routine = 0;
		      if (sscanf (sym->str, localFormat, &area, &label, &i, &routine) > 2)
			{
			  localFindRout (routine, &file, &lineno);
			  errorLine (lineno, file, ErrorError, FALSE, "Missing local label (fwd) with ID %02i in routine '%s'%s", label, *routine ? routine : "<anonymous>", lineno ? " in block starting" : " (unknown location)");
			}
		      else if (sscanf (sym->str + 7, "%i$$%s", &i, routine) > 0)
			{
			  localFindLocal (i, &file, &lineno);
			  errorLine (lineno, file, ErrorError, FALSE, "Missing local label '%s'%s", routine ? routine : "<anonymous>", lineno ? " in block starting" : " (unknown location)");
			}
		      return 0;
		    }
		  else
		    {
		      sym->offset = strsize;
		      strsize += sym->len + 1;
		      sym->used = nosym++;
		    }
		}
	      else
		{
		  sym->type &= ~SYMBOL_REFERENCE;
		  sym->used = -1;
		}
	    }
	}
    }
  stringtablesize = strsize;
  return nosym;
}

int
symbolStringSize (void)
{
  if (stringtablesize < 0)
    error (ErrorSerious, FALSE, "Internal symbolStringSize: bad string table size");
  return stringtablesize;
}

void
symbolStringOutput (FILE * outfile)	/* Count already output */
{
  int i;
  Symbol *sym;

  for (i = 0; i < SYMBOL_TABLESIZE; i++)
    for (sym = symbolTable[i]; sym; sym = sym->next)
      {
/*printf("%-40s  %3i %8X %8X %8X\n",sym->str,sym->used,sym->type,sym->value.Tag.t,sym->value.Tag.v); */
	if (SYMBOL_OUTPUT (sym) /*(sym)->used >= 0 */  || sym->type == SYMBOL_AREA)
	  {
/*puts("  (written)"); */
	    if (pedantic && sym->declared == 0)
	      if ((sym->type & SYMBOL_DEFINED) > 0 || sym->used > -1)
		errorLine (0, 0, ErrorWarning, TRUE, "Symbol %s is implicitly imported", sym->str);
	    fwrite ((void *) sym->str, 1, sym->len + 1, outfile);
	  }
      }

}

void
symbolSymbolOutput (FILE * outfile)
{
  int i;
  Symbol *sym;
  Value value;
  int v = 0;
  AofSymbol asym;

  for (i = 0; i < SYMBOL_TABLESIZE; i++)
    for (sym = symbolTable[i]; sym; sym = sym->next)
      if (sym->type != SYMBOL_AREA && SYMBOL_OUTPUT (sym) /*sym->used >= 0 */ )
	{
	  asym.Name = sym->offset;
	  if (sym->type & SYMBOL_DEFINED)
	    {
	      if (sym->value.Tag.t == ValueCode)
		{
		  codeInit ();
		  value = codeEvalLow (ValueAll, sym->value.ValueCode.len, sym->value.ValueCode.c);
		}
	      else
		{
		  value = sym->value;
		}
	      switch (value.Tag.t)
		{
		case ValueIllegal:
		  errorLine (0, 0, ErrorError, TRUE, "Symbol %s cannot be evaluated", sym->str);
		  v = 0;
		  break;
		case ValueInt:
		case ValueAddr:	/* nasty hack */
		  v = value.ValueInt.i;
		  break;
		case ValueFloat:
		  errorLine (0, 0, ErrorError, TRUE, "Linker does not understand float constants (%s)", sym->str);
		  v = (int) value.ValueFloat.f;
		  break;
		case ValueString:
		  v = lexChar2Int (FALSE, value.ValueString.len, value.ValueString.s);
		  break;
		case ValueBool:
		  v = value.ValueBool.b;
		  break;
		case ValueCode:
		  errorLine (0, 0, ErrorError, TRUE, "Linker does not understand code constants (%s)", sym->str);
		  v = 0;
		  break;
		case ValueLateLabel:
		  if (!value.ValueLate.late->next &&	/* Only one late label */
		      value.ValueLate.late->factor == 1 &&	/* ... occuring one time */
		      value.ValueLate.late->symbol->type == SYMBOL_AREA)
		    {		/* ... and it is an area */
		      if (sym->type & SYMBOL_ABSOLUTE)
			{	/* Change absolute to relative */
			  sym->type &= ~SYMBOL_ABSOLUTE;
			  v = value.ValueLate.i;
			  sym->area.ptr = value.ValueLate.late->symbol;
			}
		      else if (sym->area.ptr != value.ValueLate.late->symbol)
			errorLine (0, 0, ErrorError, TRUE, "Linker cannot have 2 areas for the same symbol (%s)", sym->str);
		    }
		  else
		    {
		      errorLine (0, 0, ErrorError, TRUE, "Linker cannot have many late labels for the same symbol (%s)", sym->str);
		    }
		  break;
		default:
		  errorLine (0, 0, ErrorSerious, FALSE, "Internal symbolSymbolOutput: not possible (%s) (0x%x)", sym->str, value.Tag.t);
		}
	      asym.Value = v;
	      if ((asym.Type = sym->type) & SYMBOL_ABSOLUTE)
		asym.AreaName = 0;
	      else
		asym.AreaName = sym->area.ptr->offset;
	    }
	  else
	    {
	      asym.Type = sym->type | TYPE_REFERENCE;
	      asym.Value = 0;
	      asym.AreaName = 0;
	    }
          asym.Name     = armword (asym.Name);
          asym.Type     = armword (asym.Type);
          asym.Value    = armword (asym.Value);
          asym.AreaName = armword(asym.AreaName);
	  fwrite ((void *) &asym, sizeof (AofSymbol), 1, outfile);
	}
}

static int
findAreaIndex (struct AREA * area) {
  Symbol *ap;
  int i=3;

  ap=areaHead;

  while (ap) {
    if (area == ap) {
      return i;
    }
    i++;
    if ((ap->area.info->norelocs)>0) {
      i++; /* We will be outputting a relocation section as well */
    }
    ap=ap->area.info->next;
  }
  return -1;
}

void
symbolSymbolElfOutput (FILE * outfile, int noareas)
{
  int i;
  int bind, type;
  Symbol *sym;
  Value value;
  int v = 0;
  Elf32_Sym asym;

  /* Output the undefined symbol */
  asym.st_name=0;
  asym.st_value=0;
  asym.st_size=0;
  asym.st_info=0;
  asym.st_other=0;
  asym.st_shndx=0;

  fwrite ((void *) &asym, sizeof (Elf32_Sym), 1, outfile);

  /* Output area symbols */
  for (i = 0; i < noareas; i++) {
    asym.st_info = ELF32_ST_INFO(STB_LOCAL, STT_SECTION);
    asym.st_shndx = i + 3;

    fwrite ((void *) &asym, sizeof (Elf32_Sym), 1, outfile);
  }

  for (i = 0; i < SYMBOL_TABLESIZE; i++)
    for (sym = symbolTable[i]; sym; sym = sym-> next)
      if (sym->type != SYMBOL_AREA && SYMBOL_OUTPUT (sym) /*sym->used >= 0 */ )
        {
          asym.st_name = sym->offset - 3;
          type = 0;
          bind = 0;
          if (sym->type & SYMBOL_DEFINED)
            {
              if (sym->value.Tag.t == ValueCode)
                {
                  codeInit ();
                  value = codeEvalLow (ValueAll, sym->value.ValueCode.len, sym->value.ValueCode.c);
                  type = STT_NOTYPE; /* No information to base type on */
                }
              else
                {
                  value = sym->value;
                }
              switch (value.Tag.t)
                {
                case ValueIllegal:
                  errorLine (0, 0, ErrorError, TRUE, "Symbol %s cannot be evaluated", sym->str);
                  v = 0;
                  break;
                case ValueInt:
                case ValueAddr: /* nasty hack */
                  v = value.ValueInt.i;
                  break;
                case ValueFloat:
                  errorLine (0, 0, ErrorError, TRUE, "Linker does not understand float constants (%s)", sym->str);
                  v = (int) value.ValueFloat.f;
                  break;
                case ValueString:
                  v = lexChar2Int (FALSE, value.ValueString.len, value.ValueString.s);
                  break;
                case ValueBool:
                  v = value.ValueBool.b;
                  break;
                case ValueCode:
                  errorLine (0, 0, ErrorError, TRUE, "Linker does not understand code constants (%s)", sym->str);
                  v = 0;
                  break;
                case ValueLateLabel:
                  if (!value.ValueLate.late->next &&    /* Only one late label */
                      value.ValueLate.late->factor == 1 &&      /* ... occuring
one time */
                      value.ValueLate.late->symbol->type == SYMBOL_AREA)
                    {           /* ... and it is an area */
                      if (sym->type & SYMBOL_ABSOLUTE)
                        {       /* Change absolute to relative */
                          sym->type &= ~SYMBOL_ABSOLUTE;
                          v = value.ValueLate.i;
                          sym->area.ptr = value.ValueLate.late->symbol;
                        }
                      else if (sym->area.ptr != value.ValueLate.late->symbol)
                        errorLine (0, 0, ErrorError, TRUE, "Linker cannot have 2 areas for the same symbol (%s)", sym->str);
                    }
                  else
                    {
                      errorLine (0, 0, ErrorError, TRUE, "Linker cannot have many late labels for the same symbol (%s)", sym->str);
                    }
                  break;
                default:
                  errorLine (0, 0, ErrorSerious, FALSE, "Internal symbolSymbolOutput: not possible (%s) (0x%x)", sym->str, value.Tag.t);
                }
              asym.st_value = v;
              if (sym->type & SYMBOL_ABSOLUTE)
                {
                  asym.st_shndx = 0;
                }
              else
                /* Inefficient, needs fixing later */
                asym.st_shndx = findAreaIndex(sym->area.info);
            }
          else
            {
              asym.st_value = 0;
              asym.st_shndx = 0;
            }

          if (SYMBOL_KIND(sym->type) == TYPE_LOCAL)
            bind = STB_LOCAL;

          if (SYMBOL_KIND(sym->type) == TYPE_GLOBAL ||
              SYMBOL_KIND(sym->type) == TYPE_REFERENCE)
            bind = STB_GLOBAL;

          if (SYMBOL_KIND(sym->type) == TYPE_REFERENCE &&
              (sym->type & TYPE_WEAK) > 0)
            bind = STB_WEAK;

          asym.st_info = ELF32_ST_INFO(bind, type);
          fwrite ((void *) &asym, sizeof (Elf32_Sym), 1, outfile);
        }
}
