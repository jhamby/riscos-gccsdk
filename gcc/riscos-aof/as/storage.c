/*
 * storage.c
 * Copyright © 1992 Niklas Röjemo
 */
#include "sdk-config.h"
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "storage.h"
#include "value.h"
#include "get.h"
#include "lex.h"
#include "input.h"
#include "expr.h"
#include "error.h"

static int storageD = 0;
static Value storageV;

extern int pedantic;
Value 
storageValue (void)
{
  if (!storageD)
    {
      error (ErrorError, TRUE, "No storage declared (# or @ before ^)");
      storageV.Tag.t = ValueAddr;
      storageV.ValueAddr.i = 0;
      storageV.ValueAddr.r = 15;
    }
  return valueCopy (storageV);
}

void 
c_record (void)
{
  Value value;
  storageD = TRUE;
  exprBuild ();
  value = exprEval (ValueInt);
  storageV.ValueAddr.r = 15;
  storageV.Tag.t = ValueAddr;
  switch (value.Tag.t)
    {
    case ValueInt:
      storageV.ValueAddr.i = value.ValueInt.i;
      break;
    default:
      storageV.ValueAddr.i = 0;
      error (ErrorError, FALSE, "^ cannot evaluate its offset expression");
    }
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
      storageV.ValueAddr.r = getCpuReg ();
    }
}

void 
c_alloc (Symbol * sym)
{
  Value value;
  if (sym)
    {
      sym->type |= SYMBOL_ABSOLUTE | SYMBOL_DEFINED;
      sym->area.ptr = 0;
      sym->value = storageValue ();
    }
  exprBuild ();
  value = exprEval (ValueInt);
  switch (value.Tag.t)
    {
    case ValueInt:
      if (value.ValueInt.i >= 0)
	{
	  if (pedantic > 1 && value.ValueInt.i == 0)
	    error (ErrorInfo, TRUE, "You are reserving zero bytes?");
	  storageV.ValueAddr.i += value.ValueInt.i;
	  /* ValueInt & ValueAddr have i in the same place */
	  value.ValueAddr.r = storageV.ValueAddr.r;
	}
      else
	error (ErrorError, TRUE, "Cannot reserve negative amount of space %d", value.ValueInt.i);
      break;
    default:
      error (ErrorError, FALSE, "Illegal expression after #");
      break;
    }
}
