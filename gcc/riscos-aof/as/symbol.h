
/*
 *   symbol.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef _symbol_h
#define _symbol_h

#include <stdio.h>

#ifndef _lex_h
#include "lex.h"
#endif

#ifndef _value_h
#include "value.h"
#endif

#define SYMBOL_LOCAL     0x01	/* Defined with local scope */
#define SYMBOL_REFERENCE 0x02
#define SYMBOL_GLOBAL    0x03	/* Defined with global scope */

#define SYMBOL_KIND(x)      ((x) & 0x03)

#define SYMBOL_DEFINED   0x01
#define SYMBOL_EXPORT    0x02

#define SYMBOL_ABSOLUTE  0x04	/* This is a constant, (not valid if SYMBOL_REFERENCE) */
#define SYMBOL_NOCASE    0x08	/* Only if SYMBOL_REFERENCE, case insesitive */
#define SYMBOL_WEAK      0x10	/* Only if SYMBOL_REFERENCE, must not be resolved */
#define SYMBOL_STRONG    0x20	/* Complicated ??? */
#define SYMBOL_COMMON    0x40
#define SYMBOL_KEEP	 0x80


#define SYMBOL_AREA        0x100
#define SYMBOL_NOTRESOLVED 0x200
/* Symbol is defined in a based area.  */
#define SYMBOL_BASED	   0x400

#define SYMBOL_CPUREG      0x1000
#define SYMBOL_FPUREG      0x2000
#define SYMBOL_COPREG      0x4000
#define SYMBOL_COPNUM	   0x8000
#define SYMBOL_GETREG(x)   ((x)&0xf000)

#define SYMBOL_TABLESIZE 1024


typedef struct SYMBOL
{
  struct SYMBOL *next;
  unsigned int type;
  int declared;
  Value value;
  union
  {
    struct SYMBOL *ptr;
    struct AREA *info;
  }
  area;
  unsigned int offset;		/* Offset in stringtable */
  int used;		/* this id is used (ie not resolved) */
  /* Later changed to index in symbol table */
  int len;		/* length of str[] */
  char str[1];			/* str[len+1] */
}
Symbol;


extern Symbol *symbolAdd (Lex l);
extern Symbol *symbolGet (Lex l);
extern Symbol *symbolFind (Lex l);
extern void symbolPrint (void);

extern int symbolFix (void);		/* Returns number of symbols */
extern int symbolStringSize (void);
extern void symbolStringOutput (FILE * outfile);
extern void symbolSymbolOutput (FILE * outfile);
extern void symbolSymbolElfOutput (FILE * outfile);

#endif
