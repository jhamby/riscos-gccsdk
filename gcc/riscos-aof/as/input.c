/*
 *  input.c
 * Copyright © 1992 Niklas Röjemo
 */

#include "sdk-config.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "error.h"
#include "filestack.h"
#include "global.h"
#include "input.h"
#include "lex.h"
#include "macros.h"
#include "main.h"
#include "os.h"

#define MAX_LINE (4096)

FILE *asmfile;
long int inputLineNo;
BOOL inputExpand = TRUE;
BOOL inputRewind = FALSE;
const char *inputName = NULL;

static char *input_buff = NULL;
static char *input_pos, *input_mark;
static char workBuff[MAX_LINE + 1]; /* holds each line from input file */

static BOOL inputArgSub (void);

/* Debug only:
 */
const char *inputGiveRestLine(void)
{
  return input_pos;
}

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
  const int c = *input_pos;
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


/* return char |c| to |input_buff| at position pointed to by |input_pos| */
void
inputUnGet (char c)
{
  if (input_pos > input_buff && input_pos[-1] == c)
    input_pos--;
  else if (*input_pos || c) {
    printf("char = '%c' \"%s\" \"%s\"\n", c, input_pos, input_buff);
    error (ErrorSerious, FALSE, "Internal inputUnGet: illegal character");
  }
}

/* return char |c| to position pointed to by |input_pos| */
void
inputPutBack (char c)
{
  if (input_pos[-1] == c)
    input_pos--;
  else if (*input_pos || c) {
    printf("char = '%c' \"%s\" \"%s\"\n", c, input_pos, input_buff);
    error (ErrorSerious, FALSE, "Internal inputPutBack: illegal character");
  }
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
  char *p = input_pos - 1;
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
inputInit (const char *infile)
{
  /* 256 bytes of overflow */
  if (input_buff == NULL
      && (input_buff = (char *) malloc (MAX_LINE + 256)) == NULL)
    errorOutOfMem("inputInit");

  if (infile != NULL && strcmp (infile, "-"))
    {
      if ((asmfile = fopen (infile, "r")) == NULL)
	errorLine (0, NULL, ErrorAbort, FALSE, "AS can't read %s: %s",
	           infile, strerror (errno));
      else
	{
	  char *buffer;
#ifndef CROSS_COMPILE
	  inputName = CanonicalisePath (infile);
#else
	  inputName = infile;
#endif
	  if ((buffer = (char *) malloc (17 * 1024)) == NULL)
	    errorOutOfMem("inputInit");
	  setvbuf (asmfile, buffer, _IOFBF, 16 * 1024);
	}
    }
  else
    asmfile = stdin;

  inputLineNo = 0;
  skiprest ();
}


void
inputFinish (void)
{
  if (asmfile && asmfile != stdin)
    fclose (asmfile);
}


/******************************************************************
* Read a line from the input file into file global |workBuff|, with some
*   minimal error checking.
* Application global |inputLineNo| is incremented for each line read
*
* Any empty line and any line starting with '#' followed by space and
* one or more digits is discarded.  The '#' exception is to be able to
* parse C preprocessed assembler files.
*
* If file global |inputExpand| is not set,
*   then workBuff is copied into application global |input_buff|.
*   Application global |input_pos| is a pointer to this.
*
* InputArgSub() then performs any required argument substitution
*
******************************************************************/

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
      /* If we start the line with a '#' followed by a space and one or
       * more digits then treat the whole line as a comment.
       * Like: # 5 "lib1funcs-aof.S"
       * FIXME: We could do better by using the digits as line number and
       * the next argument as a file reference of the file before
       * preprocessing.
       */
      if (workBuff[0] == '#' && workBuff[1] == ' ')
	{
	  int i;
	  for (i = 2; workBuff[i] != '\0' && isdigit(workBuff[i]); ++i)
	    ;
	  if (i > 2 && workBuff[i] == ' ')
	    {
	      (input_pos = input_buff)[0] = 0;
	      return TRUE;
	    }
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

/****************************************************************
* copy the line from |workBuff| to |input_buff|, while performing any
* substitutions.
*
* Retrns TRUE if successful
*
****************************************************************/

static BOOL
inputArgSub (void)
{
  Lex label;
  Symbol *sym;
  int ptr = 0, trunc = 0, len;
  char g, c, *rb;

  g = gcc_backend ? '@' : 0;
  input_pos = workBuff;


  /* process all characters in the line */
  while (*input_pos && ptr < MAX_LINE)
    {
      /* copy each input character, until a special symbol is found */
      while (*input_pos && *input_pos != '"' && *input_pos != '\''
             && *input_pos != '|' && *input_pos != '$'
	     && *input_pos != ';' && (*input_pos != '<' || !objasm)
	     && *input_pos != g && ptr < MAX_LINE)
	input_buff[ptr++] = *input_pos++;

      /* process special characters */
      switch (c = *input_pos)
	{

	/* comment follows; just copy it all */
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

	/* characters enclosed between <...>
	   Not ObjAsm; I don't know what this means?
	*/
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

	/* Do variable substitution - $
	*/
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

	  /* replace symbol by its definition */
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
	  sym = symbolFind (&label);
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
		    const char *s;
		    if ((s = strndup(label.LexId.str, label.LexId.len)) == NULL)
		      {
		        errorOutOfMem("inputArgSub");
		        return FALSE;
		      }
		    error (ErrorError, TRUE, "$ expansion '%s' is a pointer",
			   s);
		    free((void *)s);
		  }
		  break;
		default:
		  goto unknown;
		}

		/* substitution complete; copy the rest of the line */
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
	      const char *s;
	    unknown:
	      if ((s = strndup(label.LexId.str, label.LexId.len)) == NULL)
	        {
	          errorOutOfMem("inputArgSub");
	          return FALSE;
	        }
	      error (ErrorError, TRUE, "Unknown value '%s' for $ expansion",
		     s);
	      free((void *)s);
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
      /* We do allow labels beginning with '#' */
      if (*p == '#')
        ++p;
      while ((c = *p) != 0
             && (isalnum (c)
		 || c == '_'
		 || (c == '$'
		     && local
		     && (p[1] == 'l' || p[1] == 'L')
		     && p[2] != '.'
		     && p[2] != '_'
		     && !isalpha (p[2]))))
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
inputThisInstead (const char *p)
{
  strcpy (input_pos = input_mark = input_buff, p);
}
