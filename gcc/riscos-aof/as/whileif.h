
/*
 * whileif.h
 * Copyright © 1997 Darren Salt
 */

#ifndef _whileif_h
#define _whileif_h

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
  char *expr;
  union
  {
    struct
    {
      long int fileno;
      long int offset;
    }
    file;
    struct
    {
      long int callno;
      char *offset;
    }
    macro;
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

void whileFree (void);

void c_while (Lex *);
void c_wend (Lex *);

void testUnmatched (void);

#endif
