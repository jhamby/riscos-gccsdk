/*
 * code.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef _code_h
#define _code_h

#ifndef _global_h
#include "global.h"
#endif

#ifndef _lex_h
#include "lex.h"
#endif

#ifndef _symbol_h
#include "symbol.h"
#endif


#ifndef _value_h
#include "value.h"
#endif

typedef enum
{
  CodeOperator,
  CodeValue,
  CodeSymbol
}
CodeTag;

typedef union CODE
{
  CodeTag Tag;
  struct
  {
    CodeTag Tag;
    Operator op;
  }
  CodeOperator;
  struct
  {
    CodeTag Tag;
    Value value;
  }
  CodeValue;
  struct
  {
    CodeTag Tag;
    Symbol *symbol;
  }
  CodeSymbol;
}
Code;

#ifndef _reloc_h
#include "reloc.h"
#endif

#define CODE_SIZECODE  1024
#define CODE_SIZESTACK 1024
#define CODE_SIZELATE  1024

void codeInit (void);


void codeOperator (Operator op);
void codeSymbol (Symbol * symbol);
void codeInt (int value);
void codePosition (Symbol * area);
void codeStorage (void);
void codeString (int len, char *str);
void codeFloat (FLOAT value);
void codeBool (BOOL value);

void codeFixReloc (void);
Value codeEvalLow (ValueTag legal, int size, Code * program);
Value codeEval (ValueTag legal);

LateInfo *codeNewLateInfo (Symbol * symbol);
Code *codeCopy (int len, Code * code);
int codeEqual (int len, Code * a, Code * b);
#endif
