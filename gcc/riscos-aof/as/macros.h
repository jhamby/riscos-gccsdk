/*
 *  macros.h
 * Copyright © 1997 Darren Salt
 */

#ifndef macros_header_included
#define macros_header_included

#include "global.h"
#include "lex.h"
#include "whileif.h"

#define MACRO_ARG0  16
#define MACRO_ARG15 31
/* These delimit a block of 16 consecutive character codes which cannot
 * appear in the assembler source
 */

typedef struct Macro
{
  struct Macro *next;
  char *name;
  const char *file;
  char *buf;
  unsigned int labelarg:8,	/* 0 or 1 of these */
    numargs:8;			/* and up to 16 in total */
  char *args[16];
  long int startline;
}
Macro;

typedef struct MacroStack
{
  Macro *macro;
  long int callno;
  const char *offset;		/* Filled in for *current* macro */
  long int lineno;		/* Return to this line... */
  WhileBlock *whilestack;
  int if_depth;
  char *args[16];
}
MacroStack;

extern int macroSP;
extern MacroStack macroStack[10];
extern char *macroArgs[16];
extern Macro *macroCurrent;
extern const char *macroPtr;
extern long int macroCurrentCallNo;

Macro *macroFind (size_t len, char *);
void macroCall (Macro *, Lex *);
BOOL macroGetLine (char *buf);	/* returns 0 if end of macro */

BOOL macroAdd (Macro *);

void c_macro (Lex *);		/* start of macro definition */
void c_mexit (Lex *);

#endif
