/*
 *  input.c
 * Copyright © 1992 Niklas Röjemo
 */

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include "error.h"
#include "global.h"
#include "input.h"
#include "os.h"
#include "filestack.h"
#include "uname.h"
#include "macros.h"
#include "lex.h"

#define MAX_LINE 4096

extern int gcc_backend;
extern int dde;
extern int objasm;
extern int local;		/* these externs from main.c */
extern int pedantic;

FILE *asmfile;

static char workBuff[MAX_LINE + 1];
char *inputName = NULL;

long int inputLineNo;
BOOL inputExpand = TRUE;
BOOL inputRewind = FALSE;

/* Keep global as these are used by the inline versions of some input
   functions.  */
char *input_buff = NULL;
char *input_pos, *input_mark;

char
inputLook (void)
{
  return *input_pos;
}


char
inputLookLower (void)
{
  return tolower (*input_pos);
}

char
inputLookUC (void)
{
  char x = *input_pos;
  return uc ? FLIP (x) : tolower (x);
}

int
inputComment (void)
{
  int c = *input_pos;
  return c == 0 || c == ';' || (gcc_backend && c == '@');
}


char
inputLookN (int n)		/* Unsafe */
{
  return input_pos[n];
}


char
inputLookNLower (int n)		/* Unsafe */
{
  return tolower (input_pos[n]);
}

char
inputLookNUC (int n)		/* Unsafe */
{
  char x = input_pos[n];
  return uc ? FLIP (x) : tolower (x);
}

char
inputGet (void)
{
  if (*input_pos)
    return *input_pos++;
  else
    return *input_pos;
}


char
inputGetLower (void)
{
  char c = *input_pos ? *input_pos++ : *input_pos;
  return tolower (c);
}

char
inputGetUC (void)
{
  char x = *input_pos ? *input_pos++ : *input_pos;
  return uc ? FLIP (x) : tolower (x);
}

void
inputUnGet (char c)
{
  if (input_pos > input_buff && input_pos[-1] == c)
    input_pos--;
  else if (*input_pos || c)
    error (ErrorSerious, FALSE, "Internal inputUnGet: illegal character");
}


void
inputSkip (void)
{
  if (*input_pos)
    input_pos++;
}


void
inputSkipN (int n)
{
  while (*input_pos && n--)
    input_pos++;
  if (n > (*input_pos ? -1 : 0))	/* Fix to allow skip to end of string */
    error (ErrorSerious, FALSE, "Internal inputSkipN: trying to skip more characters than are available");
}


char *
inputRest (void)
{
  char *t = input_pos;
  register char *p = input_pos - 1;
  while (*++p);
  input_pos = p;
  return t;
}


char *
inputLine (void)
{
  return input_buff;
}


char
inputSkipLook (void)
{
  return *input_pos ? *++input_pos : *input_pos;
}


void
skipblanks (void)
{
  char *p = input_pos;
  while (*p && isspace (*p))
    p++;
  input_pos = p;
}


void
skiprest (void)
{
  (input_pos = input_mark = input_buff)[0] = 0;
}


void
inputMark (void)
{
  input_mark = input_pos;
}


void
inputRollback (void)
{
  input_pos = input_mark;
}

void
inputInit (char *infile)
{
  if (input_buff == NULL)
    {
      /* 256 bytes of overflow */
      input_buff = (char *) malloc (MAX_LINE + 256);
    }
  if (infile && strcmp (infile, "-"))
    {
#if defined(UNIXLIB) || defined(CROSS_COMPILE)
      if ((asmfile = fopen (infile, "r")) == NULL)
	errorLine (0, 0, ErrorAbort, FALSE, "AS can't read %s: %s", infile,
		   strerror (errno));
#else
      if ((asmfile = fopen (uname (infile, dde), "r")) == NULL)
	errorLine (0, 0, ErrorAbort, FALSE, "AS can't read %s", infile);
#endif
      else
	{
	  char *buffer;
#ifndef CROSS_COMPILE
	  inputName = CanonicalisePath (infile);
#else
	  inputName = infile;
#endif
	  buffer = (char *) malloc (17 * 1024);
	  setvbuf (asmfile, buffer, _IOFBF, 16 * 1024);
	}
    }
  else
    asmfile = stdin;
/*  macroCurrent=malloc(sizeof(Macro));  sets up a dummy macro
   if (!macroCurrent)
   errorLine(0,0,ErrorAbort,FALSE,"Not enough memory",infile);
   macroCurrent->name="";
   macroCurrent->file=(char*)inputName;
 */
  inputLineNo = 0;
  skiprest ();
}


void
inputFinish (void)
{
  if (asmfile && asmfile != stdin)
    fclose (asmfile);
}


BOOL inputArgSub (void);

BOOL
inputNextLine (void)
{
  int l;
  if (inputRewind)
    {
      inputRewind = FALSE;
      goto ret;
    }
  if (asmfile == NULL)
    goto retBad;
  inputLineNo++;
  if (macroCurrent && macroGetLine (workBuff))
    goto ret;
  while (fgets (workBuff, MAX_LINE, asmfile) == NULL)
    {
      if (pedantic)
	error (ErrorWarning, TRUE, "No END found in this file");
      if ((asmfile = pop_file ()) == NULL)
	{
	retBad:
	  inputLineNo = -inputLineNo;
	  return FALSE;
	}
    }
  l = strlen (workBuff);
  if (l)
    {
      /* If we start the line with a '#' then treat the whole
       * line as a comment.
       */
      if (workBuff[0] == '#')
	{
	  (input_pos = input_buff)[0] = 0;
	  return TRUE;
	}
      if (workBuff[l - 1] != '\n')
	{
	  if ((l = getc (asmfile)) != EOF && l != '\n')
	    {
	      error (ErrorSerious, TRUE, "Line truncated");
	      while ((l = getc (asmfile)) != EOF && l != '\n');
	    }
	}
      else
	workBuff[l - 1] = 0;
    }
ret:
  if (!inputExpand)
    {
      strcpy (input_pos = input_buff, workBuff);
      return TRUE;
    }
  return inputArgSub ();
}


BOOL
inputArgSub (void)
{
  Lex label;
  Symbol *sym;
  int ptr = 0, trunc = 0, len;
  char g, c, *rb;

  g = gcc_backend ? '@' : 0;
  input_pos = workBuff;
  while (*input_pos && ptr < MAX_LINE)
    {
      while (*input_pos && *input_pos != '"' && *input_pos != '\''
             && *input_pos != '|' && *input_pos != '$'
	     && *input_pos != ';' && (*input_pos != '<' || !objasm)
	     && *input_pos != g && ptr < MAX_LINE)
	input_buff[ptr++] = *input_pos++;

      switch (c = *input_pos)
	{
	case ';':
	case '@':
	  len = strlen (input_pos);
	  if (ptr + len >= MAX_LINE)
	    {
	      memcpy (input_buff + ptr, input_pos, MAX_LINE - ptr - len);
	      goto truncated;
	    }
	  strcpy (input_buff + ptr, input_pos);
	  goto finished;
	case '<':		/* is it a variable name? */
	  rb = input_pos;
	  while (*++rb != 0 && *rb != '>' && *rb > 32);
	  if (*rb != '>')
	    {
	      input_buff[ptr++] = *input_pos++;
	      break;
	    }
	  c = '>';		/* set delimiter and fall through */
	case '|':
	  do
	    {
	      input_buff[ptr++] = *input_pos++;
	    }
	  while (*input_pos && *input_pos != c && ptr < MAX_LINE);
	  if (*input_pos == c)
	    {
	      input_buff[ptr++] = c;
	      input_pos++;
	    }
	  break;
	case '\'':
	  if (objasm)
	    input_buff[ptr++] = *input_pos++;	/* some special case stuff */
	  /* fall through to '"' */
	case '\"':
	  do
	    {
	      char cc = input_buff[ptr++] = *input_pos++;
	      if (cc == '\\' && *input_pos)
		input_buff[ptr++] = *input_pos++;
	    }
	  while (*input_pos && *input_pos != c && ptr < MAX_LINE);
	  if (*input_pos == c)
	    {
	      input_buff[ptr++] = c;
	      input_pos++;
	    }
	  break;
	case '$':
/*        if (!inputExpand) {
   input_buff[ptr++]='$';
   if (*++input_pos=='$') {
   input_buff[ptr++]='$'; break;
   } else {
   char *c=inputSymbol(&len,0);
   memcpy(input_buff+ptr,c,len);
   ptr+=len;
   }
   break;
   } else */
	  rb = input_pos;
	  len = ptr;
	  if (*++input_pos == '$')
	    {
	      input_buff[ptr++] = '$';
	      input_pos++;
	      break;
	    }
	  label = lexGetId ();
	  if (label.tag != LexId)
	    {
	      error (ErrorWarning, TRUE, "Non-ID in $ expansion");
	      input_buff[ptr++] = '$';
	      break;
	    }
	  if (c && *input_pos == '.')
	    input_pos++;
	  if (local && label.LexId.len == 1 && toupper (*label.LexId.str) == 'L')
	    {
	      input_buff[ptr++] = '$';
	      input_buff[ptr++] = *label.LexId.str;
	      break;
	    }
	  sym = symbolFind (label);
	  if (sym)
	    {
	      switch (sym->value.Tag.t)
		{
		case ValueInt:
		  sprintf (input_buff + ptr, "%i", sym->value.ValueInt.i);
		  ptr += strlen (input_buff + ptr);
		  break;
		case ValueFloat:
		  sprintf (input_buff + ptr, "%f", sym->value.ValueFloat.f);
		  ptr += strlen (input_buff + ptr);
		  break;
		case ValueString:
		  if (ptr + sym->value.ValueString.len >= MAX_LINE)
		    ptr = MAX_LINE + 1;
		  else
		    {
		      strcpy (input_buff + ptr, sym->value.ValueString.s);
		      ptr += sym->value.ValueString.len;
		    }
		  break;
		case ValueBool:
		  strcpy (input_buff + ptr, sym->value.ValueBool.b ? "{TRUE}" : "{FALSE}");
		  ptr += strlen (input_buff + ptr);
		  break;
		case ValueCode:
		case ValueLateLabel:
		case ValueAddr:
		  {
		    char c = label.LexId.str[label.LexId.len];
		    label.LexId.str[label.LexId.len] = 0;
		    error (ErrorError, TRUE, "$ expansion '%s' is a pointer",
			   label.LexId.str);
		    label.LexId.str[label.LexId.len] = c;
		  }
		  break;
		default:
		  goto unknown;
		}
	      while (*input_pos && ptr < MAX_LINE)
		input_buff[ptr++] = *input_pos++;
	      if (ptr >= MAX_LINE)
		{
		  trunc = 1;
		  input_buff[MAX_LINE - 1] = 0;
		}
	      else
		input_buff[ptr] = 0;
	      ptr = len;
	      strcpy (input_pos = rb, input_buff + ptr);	/* only copy what's necessary */
	    }
	  else
	    {
	      char c;
	    unknown:c = label.LexId.str[label.LexId.len];
	      label.LexId.str[label.LexId.len] = 0;
	      error (ErrorError, TRUE, "Unknown value '%s' for $ expansion",
		     label.LexId.str);
	      label.LexId.str[label.LexId.len] = c;
	      input_buff[ptr++] = '$';
	    }
	}
    }
  if (ptr >= MAX_LINE || trunc)
    {
    truncated:
      (input_pos = input_buff)[MAX_LINE - 1] = 0;
      error (ErrorSerious, TRUE, "Line expansion truncated");
      return FALSE;
    }
finished:
  (input_pos = input_buff)[ptr] = 0;
  return TRUE;
}


char *
inputSymbol (int *ilen, char del)
{
  char *p = input_pos;
  int c;

  if (del)
    {
      if (objasm && (del == '\'' || del == '\"'))
	{
	  if (del == '\'')
	    ++p;		/* some special case stuff... */
	  while ((c = *p) != 0 && c != del)
	    p++;
	}
      else
	{
	  while ((c = *p) != 0 && c != del)
	    {
	      p++;
	      if (c == '\\' && *p)
		p++;
	    }
	}
    }
  else
    {
      while ((c = *p) != 0
             && (isalnum (c) || c == '_'
		 || (c == '$' && local && (p[1] == 'l' || p[1] == 'L')
		     && p[2] != '.' && p[2] != '_' && !isalpha (p[2]))))
	{
	  p++;
	  if (c == '\\' && *p)
	    p++;
	}
    }
  *ilen = p - input_pos;
  input_pos = p;
  return input_pos - *ilen;
}


void
inputThisInstead (char *p)
{
  strcpy (input_pos = input_mark = input_buff, p);
}
