/*
 * whileif.h
 * Copyright © 1997 Darren Salt
 */

#ifndef whileif_header_included
#define whileif_header_included

#include "symbol.h"

typedef enum
{
  WhileNull = 0,
  WhileInFile = 1,
  WhileInMacro = 2
}
WhileTag;

typedef struct WhileBlock
{
  struct WhileBlock *prev;
  WhileTag tag;
  const char *expr;	/* Ptr to malloced block holding the expression */
  union
  {
    struct
    {
      long int fileno;
      long int offset;
    }
    file;		/* Valid when tag == WhileInFile */
    struct
    {
      long int callno;
      const char *offset;
    }
    macro;		/* Valid when tag == WhileInMacro */
  }
  ptr;
  long int lineno;
}
WhileBlock;

extern int if_depth;
extern WhileBlock *whileCurrent;

void c_if (void);
void c_else (Lex *);
void c_endif (Lex *);

void c_while (Lex *);
void c_wend (Lex *);

void testUnmatched (void);

#endif
