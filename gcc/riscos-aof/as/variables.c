/*
 * variables.c
 * Copyright © 1997 Darren Salt
 */
#include "sdk-config.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "variables.h"
#include "error.h"
#include "input.h"
#include "output.h"
#include "lex.h"
#include "help_lex.h"
#include "expr.h"
#include "code.h"
#include "value.h"
#include "put.h"
#include "fix.h"
#include "include.h"
#include "filestack.h"
#include "area.h"
#include "lit.h"
#include "macros.h"
#include "strdup.h"
#include "hash.h"
#include "symbol.h"
#include "os.h"


typedef struct varPos
  {
    struct varPos *next;
    char *name;
    Symbol *symptr;
    Symbol symbol;
  }
varPos;


static varPos *varList = 0;


static char 
var_type (ValueTag type)
{
  switch (type)
    {
    case ValueInt:
      return 'I';
    case ValueBool:
      return 'L';
    case ValueString:
      return 'S';
    default:
      return '?';
    }
  return '?';
}


static void 
assign_var (Symbol * sym, ValueTag type)
{
  sym->value.Tag.t = type;
  sym->value.Tag.v = type;
  switch (type)
    {
    case ValueInt:
      sym->value.ValueInt.i = 0;
      break;
    case ValueBool:
      sym->value.ValueBool.b = FALSE;
      break;
    case ValueString:
      sym->value.ValueString.len = 0;
      sym->value.ValueString.s = malloc (1);
      sym->value.ValueString.s[0] = 0;	/* No error checking */
      break;
    default:
      abort ();
    }
}


static void 
declare_var (const char *ptr, int len, const ValueTag type, BOOL local)
{
  Lex var;
  Symbol *sym = 0;

  var = lexTempLabel ((char *) ptr, len);
  if (local == FALSE && (sym = symbolFind (var)) != NULL)
    {
      error (ErrorWarning, TRUE, "Redeclaration of variable '%*s'",
	     var.LexId.len, var.LexId.str);
      return;
    }
  /* *Will* occur for locals redeclared as globals, hence just a warning */
  if (sym)
    {
      const char *c = 0;
      if (sym->value.Tag.v == ValueConst)
	c = "symbol";
      else if (local == FALSE && sym->value.Tag.t)
	c = "variable";
      if (c)
	{
	  strncpy (er, ptr, len);
	  er[len] = 0;
	  error (ErrorError, TRUE, "'%s' is already declared as a %s", er, c);
	  inputRest ();
	  return;
	}
    }
  else
    {
      sym = symbolAdd (var);
      if (!sym)
	return;
    }
  assign_var (sym, type);
}


static char *
var_inputSymbol (int *len)
{
  char delim = '|', *sym;
  if (inputLook () == '|')
    inputSkip ();
  else
    delim = 0;
  sym = inputSymbol (len, delim, 1);
  if (delim == '|')
    {
      if (inputLook () == '|')
	inputSkip ();
      else
	error (ErrorError, TRUE, "Unterminated |label|");
    }
  return sym;
}


void 
c_gbl (ValueTag type, Lex * label)
{
  char *ptr;
  int len = 0;

  if (label->tag != LexNone)
    error (ErrorWarning, TRUE, "Label not allowed here - ignoring");
  skipblanks ();
  if (!inputComment ())
    ptr = var_inputSymbol (&len);
  if (!len)
    {
      error (ErrorError, TRUE, "Missing variable name");
      return;
    }
  declare_var (ptr, len, type, FALSE);
}


void 
c_lcl (ValueTag type, Lex * label)
{
  char *ptr;
  int len = 0;
  varPos *p;
  Symbol *sym;

  if (!macroSP)
    {
      error (ErrorError, TRUE, "Local variables not allowed outside macros");
      inputRest ();
      return;
    }
  if (label->tag != LexNone)
    error (ErrorWarning, TRUE, "Label not allowed here - ignoring");
  skipblanks ();
  if (!inputComment ())
    ptr = var_inputSymbol (&len);
  if (!len)
    {
      error (ErrorError, TRUE, "Missing variable name");
      return;
    }
  sym = symbolFind (lexTempLabel (ptr, len));
  p = malloc (sizeof (varPos));
  if (!p)
    goto nomem;
  p->next = varList;
  p->name = strndup (ptr, len);
  if (!p->name)
    goto nomem;
  p->symptr = sym;
  if (sym)
    {
      memcpy (&p->symbol, sym, sizeof (Symbol));
      assign_var (sym, type);
    }
  else
    declare_var (ptr, len, type, TRUE);
  varList = p;
  return;

nomem:
  errorOutOfMem ("c_lcl");
}


void 
c_set (ValueTag type, Lex * label)
{
  Value value;
  const char *c = 0;
  Symbol *sym = symbolFind (*label);
  if (sym == 0 || sym->value.Tag.t == ValueIllegal)
    c = "undefined";
  else if (sym->value.Tag.v == ValueConst)
    c = "not a variable";
  if (c)
    {
      strncpy (er, label->LexId.str, label->LexId.len);
      er[label->LexId.len] = 0;
      error (ErrorError, TRUE, "'%s' is %s", er, c);
      inputRest ();
      return;
    }
  exprBuild ();
  value = exprEval (sym->value.Tag.t);
  value.Tag.v = sym->value.Tag.v;
  sym->type = SYMBOL_DEFINED;
  switch (value.Tag.t)
    {
    case ValueIllegal:
      error (ErrorError, TRUE, "Illegal SET%c", var_type (type));
      sym->value.Tag.t = ValueInt;
      sym->value.ValueInt.i = 0;
      break;
#ifdef DEBUG
    case ValueString:
      printf ("String: len %i, value %s\n", value.ValueString.len, value.ValueString.s);
#endif
    default:
      sym->value = valueCopy (value);
    }
}


void 
var_restoreLocals (void)
{
  varPos *p, *q = varList;
  Symbol *sym;

  while (p = q, p)
    {
      if (p->symptr)
	{
	  if (p->symptr->value.Tag.t == ValueString)
	    free (p->symptr->value.ValueString.s);
	  memcpy (p->symptr, &p->symbol, sizeof (Symbol));
	}
      else
	{
	  sym = symbolFind (lexTempLabel (p->name, strlen (p->name)));
	  if (sym)
	    {
	      sym->type = 0;	/* undefined */
	      sym->value.Tag.t = ValueIllegal;
	    }
	}
      q = p->next;
      free (p->name);
      free (p);
    }
  varList = 0;
}


void 
var_define (const char *def)
{
  Lex var;
  Value value;
  int len;

  const char *i = strchr (def, '=');
  len = i ? i - def : strlen (def);
  declare_var (def, len, ValueString, FALSE);
  var = lexTempLabel ((char *) def, len);
  if (!i)
    i = "";
  else
    i++;
  value.Tag.t = ValueString;
  value.Tag.v = ValueString;
  value.ValueString.len = strlen (i);
  value.ValueString.s = (char *) i;
  symbolFind (var)->value = valueCopy (value);
}
