
/*
 * value.c
 * Copyright © 1992 Niklas Röjemo
 */
#include "sdk-config.h"
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "value.h"
#include "code.h"
#include "error.h"
#include "strdup.h"

/* Code demands at least one Lateinfo */
Value valueLateToCode(int offset,LateInfo *late)
{
  LateInfo *l;
  Value value;
  int factor;
  int size;

  for(size=1, l= late; l; l= l->next) {
    if((factor = l->factor) == -1 || factor == 1)
      size += 2;
    else if(factor != 0)
      size += 4;
  }
  if(offset == 0) {
    size -= 2;			     /* No need to include offset ... */
    if(late->factor == -1) size++;   /* ... but must insert an unary - */
  }
  value.ValueCode.c = malloc(size*sizeof(Code));
  value.Tag.v = ValueConst;

  if(value.ValueCode.c) {
    value.Tag.t = ValueCode;
    value.ValueCode.len = size;
    size = 0;
    if(offset != 0) {
      value.ValueCode.c[size  ].Tag = CodeValue;
      value.ValueCode.c[size  ].CodeValue.value.Tag.t = ValueInt;
      value.ValueCode.c[size++].CodeValue.value.ValueInt.i = offset;
    }
    for(l=late; l; l=l->next) {
      if((factor = l->factor) == -1 || factor == 1) {
	value.ValueCode.c[size  ].Tag = CodeSymbol;
	value.ValueCode.c[size++].CodeSymbol.symbol = l->symbol;
	if(size != 1) { /* Add offset */
	  value.ValueCode.c[size  ].Tag = CodeOperator;
	  value.ValueCode.c[size++].CodeOperator.op = (factor>0?Op_add:Op_sub);
	} else if(factor == -1) {
	  value.ValueCode.c[size  ].Tag = CodeOperator;
	  value.ValueCode.c[size++].CodeOperator.op = Op_neg;
	}
      } else if(factor != 0) {
	value.ValueCode.c[size  ].Tag = CodeSymbol;
	value.ValueCode.c[size++].CodeSymbol.symbol = l->symbol;
	value.ValueCode.c[size  ].Tag = CodeValue;
	value.ValueCode.c[size  ].CodeValue.value.Tag.t = ValueInt;
	value.ValueCode.c[size++].CodeValue.value.ValueInt.i = factor;
	value.ValueCode.c[size  ].Tag = CodeOperator;
	value.ValueCode.c[size++].CodeOperator.op = Op_mul;
	if(size != 3) { /* Add offset */
	  value.ValueCode.c[size  ].Tag = CodeOperator;
	  value.ValueCode.c[size++].CodeOperator.op = Op_add;
	}
      }
    }
  } else {
    errorOutOfMem("valueLateToCode");
    value.Tag.t = ValueIllegal;
  }
  return value;
}


Value valueCopy(Value value)
{
  switch(value.Tag.t) {
    case ValueIllegal:
    case ValueInt:
    case ValueFloat:
    case ValueBool:
    case ValueAddr:
      break;
    case ValueString:
      value.ValueString.s=strndup(value.ValueString.s,value.ValueString.len);
      break;
    case ValueCode:
      value.ValueCode.c = codeCopy(value.ValueCode.len,value.ValueCode.c);
      break;
    case ValueLateLabel:
      value = valueLateToCode(value.ValueLate.i,value.ValueLate.late);
      break;
    default:
      error(ErrorSerious,FALSE,"Internal valueCopy: illegal value");
      value.Tag.t = ValueIllegal;
  }
  return value;
}

void valueFree(Value value)
{
  switch(value.Tag.t) {
    case ValueIllegal:
    case ValueInt:
    case ValueFloat:
    case ValueBool:
    case ValueAddr: /* Needed here? */
      break;
    case ValueString:
      if (value.Tag.v==ValueString) {
	free(value.ValueString.s);
	value.ValueString.s=0;
      } else
	error(ErrorSerious,FALSE,"Internal valueFree: cannot handle %s", "string");
      break;
    case ValueCode:
      free(value.ValueCode.c);
      break;
    case ValueLateLabel:
      error(ErrorSerious,FALSE,"Internal valueFree: cannot handle %s", "late label");
      break;
    default:
      error(ErrorSerious,FALSE,"Internal valueFree: illegal value");
  }
}

static int lateInfoEqual(LateInfo *a, LateInfo *b)
{
  for(; a || b; a = a->next, b = b->next) {
    if(!a || !b)	       return FALSE;
    if(a->factor != b->factor) return FALSE;
    if(a->symbol != b->symbol) return FALSE;
  }
  return TRUE;
}

int valueEqual(Value *a, Value *b)
{
  if(a->Tag.t == ValueLateLabel) { /* Prevent valueEqual(ValueLateLabel,ValueCode) */
    Value *t = a; a = b; b = t;
  }
  switch(a->Tag.t) {
    case ValueIllegal: return b->Tag.t == ValueIllegal;
    case ValueInt:     return b->Tag.t == ValueInt    && a->ValueInt.i	 == b->ValueInt.i;
    case ValueFloat:   return b->Tag.t == ValueFloat  && a->ValueFloat.f == b->ValueFloat.f;
    case ValueBool:    return b->Tag.t == ValueBool   && a->ValueBool.b  == b->ValueBool.b;
    case ValueString:
      return b->Tag.t == ValueString &&
	a->ValueString.len  == b->ValueString.len &&
	!memcmp(a->ValueString.s,b->ValueString.s,a->ValueString.len);
    case ValueCode:
      if(b->Tag.t == ValueLateLabel) {
	Value v = valueLateToCode(b->ValueLate.i,b->ValueLate.late);
	int res = a->ValueCode.len == v.ValueCode.len &&
		  codeEqual(a->ValueCode.len,a->ValueCode.c,v.ValueCode.c);
	valueFree(v);
	return res;
      } else {
       return b->Tag.t == ValueCode &&
	      a->ValueCode.len == b->ValueCode.len &&
	      codeEqual(a->ValueCode.len,a->ValueCode.c,b->ValueCode.c);
      }
    case ValueLateLabel:
       return b->Tag.t == ValueLateLabel &&
	      a->ValueLate.i   == b->ValueLate.i &&
	      lateInfoEqual(a->ValueLate.late,b->ValueLate.late);
    default:
      error(ErrorSerious,FALSE,"Internal valueEqual: illegal value");
  }
  return FALSE;
}
