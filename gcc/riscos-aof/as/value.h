/*
 * value.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef value_header_included
#define value_header_included

#include "global.h"

struct SYMBOL;			/* Cannot include symbol.h as it needs value.h */

union CODE;

typedef struct LATEINFO
{
  struct LATEINFO *next;
  int factor;
  struct SYMBOL *symbol;
}
LateInfo;

typedef enum
{
  ValueIllegal   = 0,
  ValueInt       = 1,
  ValueFloat     = 2,
  ValueString    = 4,
  ValueBool      = 8,
  ValueCode      = 16,
  ValueLateLabel = 32,
  ValueAddr      = 64,

  ValueConst     = 0,		/* only for variables */
  ValueAll       = 127,		/* cheat */

  ValueSingle    = 1 << 28,	/* for use in literal pool */
  ValueDouble    = 3 << 28,
  ValueExtended  = 5 << 28,	/* inaccurate */
  ValuePacked    = 7 << 28	/* inaccurate */
}
ValueTag;

typedef struct
{
  ValueTag t;
  ValueTag v;			/* var type: ValueIllegal (const), ValueInt, ValueString */
}
ValueType;

typedef union
{
  ValueType Tag;
  struct
  {				/* ValueInt */
    ValueType Tag;
    int i;
  }
  ValueInt;
  struct
  {				/* ValueFloat */
    ValueType Tag;
    FLOAT f;
  }
  ValueFloat;
  struct
  {				/* ValueString */
    ValueType Tag;
    int len;
    const char *s;
  }
  ValueString;
  struct
  {				/* ValueBool */
    ValueType Tag;
    BOOL b;
  }
  ValueBool;
  struct
  {				/* ValueCode */
    ValueType Tag;
    int len;
    union CODE *c;
  }
  ValueCode;
  struct
  {				/* ValueLateLabel */
    ValueType Tag;		/* Must start identical with ValueInt */
    int i;
    struct LATEINFO *late;
  }
  ValueLate;
  struct
  {				/* ValueAddr */
    ValueType Tag;
    int i;
    int r;
  }
  ValueAddr;
}
Value;

Value valueLateToCode (int offset, LateInfo * late);
Value valueCopy (Value value);
int valueEqual (const Value * a, const Value * b);
#ifdef DEBUG
void valuePrint(const Value *v);
#endif
#endif
