/*
 * code.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef code_header_included
#define code_header_included

#include "global.h"
#include "lex.h"
#include "reloc.h"
#include "symbol.h"
#include "value.h"

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

#define CODE_SIZECODE  (1024)
#define CODE_SIZESTACK (1024)
#define CODE_SIZELATE  (1024)

extern BOOL exprNotConst;

void codeInit (void);

void codeOperator (Operator op);
void codeSymbol (Symbol * symbol);
void codeInt (int value);
void codePosition (Symbol * area);
void codeStorage (void);
void codeString (int len, const char *str);
void codeFloat (FLOAT value);
void codeBool (BOOL value);

void codeFixReloc (void);
Value codeEvalLow (ValueTag legal, int size, Code * program);
Value codeEval (ValueTag legal);

LateInfo *codeNewLateInfo (Symbol * symbol);
Code *codeCopy (int len, Code * code);
int codeEqual (int len, Code * a, Code * b);
#endif
