/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2008 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * input.c
 */

#include "config.h"
#include <assert.h>
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

#ifdef __TARGET_UNIXLIB__
#  include <unixlib/local.h>
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
const char *inputName = NULL; /**< Malloced block holding the current filename, is NUL terminated.  */

static char *input_buff = NULL;
static char *input_pos, *input_mark;
static char workBuff[MAX_LINE + 1]; /* holds each line from input file */

static BOOL inputArgSub (void);

/* For debug only.  */
const char *
inputGiveRestLine(void)
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
  return option_uc ? FLIP (x) : tolower (x);
}

/**
 * \return TRUE if next input character is NUL or start of a comment.
 */
BOOL
inputComment (void)
{
  const int c = *input_pos;
  return c == 0 || c == ';';
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
  return option_uc ? FLIP (x) : tolower (x);
}

char
inputGet (void)
{
  return (*input_pos) ? *input_pos++ : *input_pos;
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
  return option_uc ? FLIP (x) : tolower (x);
}


/* return char |c| to |input_buff| at position pointed to by |input_pos| */
void
inputUnGet (char c)
{
  if (input_pos > input_buff && input_pos[-1] == c)
    input_pos--;
  else if (*input_pos || c)
    {
      /* printf("char = '%c' \"%s\" \"%s\"\n", c, input_pos, input_buff); */
      error (ErrorSerious, FALSE, "Internal inputUnGet: illegal character");
    }
}

/* return char |c| to position pointed to by |input_pos| */
void
inputPutBack (char c)
{
  if (input_pos[-1] == c)
    input_pos--;
  else if (*input_pos || c)
    {
      /* printf("char = '%c' \"%s\" \"%s\"\n", c, input_pos, input_buff); */
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
  while (*++p)
    /* */;
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
        {
#if defined(__TARGET_UNIXLIB__)
	  /* Try again but this time with(out) suffix swapping.  */
	  __riscosify_control ^= __RISCOSIFY_NO_SUFFIX;
           asmfile = fopen (infile, "r");
	  __riscosify_control ^= __RISCOSIFY_NO_SUFFIX;
#endif
        }

      if (asmfile == NULL)
	errorLine (0, NULL, ErrorAbort, FALSE, "AS can't read %s: %s",
	           infile, strerror (errno));
      else
	{
	  free ((void *)inputName);
	  inputName = CanonicalisePath (infile);

	  static char buffer[16 * 1024];
	  setvbuf (asmfile, buffer, _IOFBF, sizeof (buffer));
	}
    }
  else
    asmfile = stdin;

  inputLineNo = 0;
  skiprest ();
}


/**
 * Close the current input file handle and optionally replace it with a new
 * given one.
 * \param newAsmFile When non-NULL, the new current input file handle.
 */
void
inputFinish (FILE *newAsmFile)
{
  if (asmfile && asmfile != stdin)
    fclose (asmfile);
  asmfile = newAsmFile;
}


/**
 * Read a line from the input file into file global |workBuff|, with some
 * minimal error checking.
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
 */
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
      if (option_pedantic)
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
	    /* */;
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
* Perform environment variable substitution (objasm compatibility mode only)
*
* input_pos points to the first input character after the <
* ptr points to the next position to write to input_buff
* trunc points to a location to receive the truncation state
*
* Returns TRUE if successful.
*
* input_pos will be updated to point to the next input character to process
* *ptr will be updated
* *trunc will be set to 1 if the input is truncated 
*
****************************************************************/

static BOOL
inputEnvSub(int *ptr, int *trunc)
{
  char *rb = input_pos;
  char *temp;
  char *env;
  int len;

  /* Find end of variable */
  while (*rb != 0 && *rb != '>' && *rb > 32)
    rb++;
  if (*rb != '>' || rb == input_pos)
    {
      /* Not a variable, had a lone '<' or "<>" */
      input_buff[(*ptr)++] = '<';
      return TRUE;
    }

  /* Clone variable name into a temporary buffer */
  temp = alloca (rb - input_pos + 1);
  memcpy (temp, input_pos, rb - input_pos);
  temp[rb - input_pos] = '\0';

  env = getenv (temp);
  if (env == NULL)
    {
      /* No such variable defined. Warn, though we may want to error. */
      error (ErrorWarning, TRUE, "Unknown environment variable '%s'", temp);
      input_buff[(*ptr)++] = '<';
      return TRUE;
    }

  len = strlen (env);

  /* Substitute variable's value, providing it won't truncate */
  if ((*ptr) + len >= MAX_LINE)
    *trunc = 1;
  else
    {
      memcpy(input_buff + (*ptr), env, len);
      (*ptr) += len;
    }

  /* Next input to process is the character after the '>' */
  /* Unlike $ substitution, we don't reprocess the substituted string */
  input_pos = rb + 1;

  return TRUE;
}

/****************************************************************
* Perform variable substitution.
*
* input_pos points to the first input character after the $
* ptr points to the next position to write to input_buff
* trunc points to a location to receive the truncation state
* inString flags whether the variable we're processing is in a string literal
*
* Returns TRUE if successful.
*
* input_pos will be updated to point to the next input character to process
* *ptr will be updated
* *trunc will be set to 1 if the input is truncated 
*
****************************************************************/

static BOOL
inputVarSub(int *ptr, int *trunc, BOOL inString)
{
  char *rb = input_pos; /* Remember input position */
  int len = *ptr;       /* And initial write offset */
  Lex label;
  Symbol *sym = NULL;

  /* $$ -> $ */
  if (*input_pos == '$')
    {
      input_buff[(*ptr)++] = '$';
      input_pos++;
      return TRUE;
    }

  /* replace symbol by its definition */
  label = lexGetIdNoError ();

  if (label.tag == LexId)
    {
      /* Skip any . after the id - it indicates concatenation (e.g. $foo.bar) */
      if (*input_pos == '.')
        input_pos++;
      /* Leave $[Ll].* alone, if we're wanting local labels */
      if (option_local && label.LexId.len == 1 && toupper (*label.LexId.str) == 'L')
        {
          input_buff[(*ptr)++] = '$';
          input_buff[(*ptr)++] = *label.LexId.str;
          return TRUE;
       }
      sym = symbolFind (&label);
    }
  else if (inString == FALSE)
    {
      /* Must be an id if we're not in a string literal */
      error (ErrorWarning, TRUE, "Non-ID in $ expansion");
      input_buff[(*ptr)++] = '$';
      /* Restore input_pos, so we reprocess the current input */
      input_pos = rb;
      /* Not a fatal error */
      return TRUE;
    }

  if (sym)
    {
      switch (sym->value.Tag.t)
	{
	case ValueInt:
	  sprintf (input_buff + (*ptr), "%i", sym->value.ValueInt.i);
	  (*ptr) += strlen (input_buff + (*ptr));
	  break;
	case ValueFloat:
	  sprintf (input_buff + (*ptr), "%f", sym->value.ValueFloat.f);
	  (*ptr) += strlen (input_buff + (*ptr));
	  break;
	case ValueString:
	  if ((*ptr) + sym->value.ValueString.len >= MAX_LINE)
	    (*ptr) = MAX_LINE + 1;
	  else
	    {
	      memcpy (input_buff + (*ptr), sym->value.ValueString.s,
                      sym->value.ValueString.len);
	      (*ptr) += sym->value.ValueString.len;
	    }
	  break;
	case ValueBool:
	  strcpy (input_buff + (*ptr), 
                  sym->value.ValueBool.b ? "{TRUE}" : "{FALSE}");
	  (*ptr) += strlen (input_buff + (*ptr));
	  break;
	case ValueCode:
	case ValueLateLabel:
	case ValueAddr:
	  error (ErrorError, TRUE, "$ expansion '%.*s' is a pointer",
		 label.LexId.len, label.LexId.str);
          /* This one's fatal */
          return FALSE;
	  break;
	default:
          goto unknown;
	}
    }
  else
    {
    unknown:
      if (inString == FALSE)
        {
          /* Not in string literal, so this is an error */
          error (ErrorError, TRUE, "Unknown value '%.*s' for $ expansion",
		 label.LexId.len, label.LexId.str);
          input_buff[(*ptr)++] = '$';
          /* Restore input_pos so we reprocess current input */
          input_pos = rb;
          /* Not a fatal error */
          return TRUE;
        }
      else
        {
          int label_len = label.tag == LexId ? label.LexId.len : 0;

          assert(label.tag == LexId || label.tag == LexNone);

          /* Unknown symbol, but in string literal, so output verbatim */
          if ((*ptr) + 1 + label_len >= MAX_LINE)
            (*ptr) = MAX_LINE + 1;
          else
            {
              input_buff[(*ptr)++] = '$';
              if (label.tag == LexId)
                memcpy (input_buff + (*ptr), label.LexId.str, label.LexId.len);
              (*ptr) += label_len;
              return TRUE;
            }
        }
    }

  /* substitution complete or not found; copy the rest of the line */
  while (*input_pos && (*ptr) < MAX_LINE)
    input_buff[(*ptr)++] = *input_pos++;
  if ((*ptr) >= MAX_LINE)
    {
      *trunc = 1;
      input_buff[MAX_LINE - 1] = 0;
    }
  else
   input_buff[(*ptr)] = 0;

  /* Restore input position and write offset */
  input_pos = rb;
  (*ptr) = len;
  /* Copy expanded string to input_pos, so it's reprocessed next time.
   * This allows variable values to contain substituted components. */
  strcpy(input_pos, input_buff + len);

  return TRUE;
}



/****************************************************************
* copy the line from |workBuff| to |input_buff|, while performing any
* substitutions.
*
* Returns TRUE if successful
*
****************************************************************/

static BOOL
inputArgSub (void)
{
  int ptr = 0, trunc = 0, len;
  char c;

  input_pos = workBuff;


  /* process all characters in the line */
  while (*input_pos && ptr < MAX_LINE)
    {
      /* copy each input character, until a special symbol is found */
      while (*input_pos && *input_pos != '"' && *input_pos != '\''
             && *input_pos != '|' && *input_pos != '$'
	     && *input_pos != ';' && (*input_pos != '<' || !option_objasm)
	     && ptr < MAX_LINE)
	input_buff[ptr++] = *input_pos++;

      /* process special characters */
      switch (c = *input_pos)
	{

	/* comment follows; just copy it all */
	case ';':
	  len = strlen (input_pos);
	  if (ptr + len >= MAX_LINE)
	    {
	      memcpy (input_buff + ptr, input_pos, MAX_LINE - ptr - len);
	      goto truncated;
	    }
	  strcpy (input_buff + ptr, input_pos);
	  goto finished;

	/* characters enclosed between <...> in ObjAsm mode */
	case '<':
          input_pos++;
          if (inputEnvSub(&ptr, &trunc) == FALSE)
            return FALSE;
          break;
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
	  if (option_objasm)
	    input_buff[ptr++] = *input_pos++;	/* some special case stuff */
	  /* fall through to '"' */
	case '\"':
	  //printf("string here: \"%s\"\n", input_pos);
	  do
	    {
	      char cc = *input_pos++;
	      if (cc == '$')
	        {
	          if (inputVarSub(&ptr, &trunc, TRUE) == FALSE)
                    return FALSE;
                  continue;
                }
              else if (cc == '<' && option_objasm)
                {
                  if (inputEnvSub(&ptr, &trunc) == FALSE)
                    return FALSE;
                  continue;
                }

              input_buff[ptr++] = cc;

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
	  input_pos++;
          if (inputVarSub(&ptr, &trunc, FALSE) == FALSE)
            return FALSE;
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
      if (option_objasm && (del == '\'' || del == '\"'))
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
		     && option_local
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
