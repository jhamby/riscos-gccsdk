/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2010 GCCSDK Developers
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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#ifdef __TARGET_UNIXLIB__
#  include <unixlib/local.h>
#endif

#include "error.h"
#include "filestack.h"
#include "input.h"
#include "macros.h"
#include "main.h"
#include "os.h"

#define MAX_LINE (4096)

static char input_buff[MAX_LINE + 256];
static const char *input_pos;
static char workBuff[MAX_LINE + 1]; /* holds each line from input file */

static bool inputArgSub (void);

#if DEBUG
const char *
inputGiveRestLine (void)
{
  return input_pos;
}
#endif

char
inputLook (void)
{
  return *input_pos;
}


char
inputLookLower (void)
{
  return tolower ((unsigned char)*input_pos);
}

/**
 * \return true if next input character is NUL or start of a comment.
 */
bool
Input_IsEolOrCommentStart (void)
{
  const char c = *input_pos;
  return c == '\0' || c == ';';
}


char
inputLookN (int n)		/* Unsafe */
{
  return input_pos[n];
}


char
inputLookNLower (int n)		/* Unsafe */
{
  return tolower ((unsigned char)input_pos[n]);
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
  return tolower ((unsigned char)c);
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
      errorAbort ("Internal inputUnGet: illegal character");
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
    errorAbort ("Internal inputSkipN: trying to skip more characters than are available");
}


/**
 * Returns the rest of the line of the current input and consumes it.
 */
const char *
inputRest (void)
{
  const char * const t = input_pos;
  while (*input_pos)
    ++input_pos;
  return t;
}

#if DEBUG
const char *
inputLine (void)
{
  return input_buff;
}
#endif

char
inputSkipLook (void)
{
  return *input_pos ? *++input_pos : *input_pos;
}


void
skipblanks (void)
{
  const char *p = input_pos;
  while (*p && isspace ((unsigned char)*p))
    p++;
  input_pos = p;
}


void
skiprest (void)
{
  input_buff[0] = 0;
  input_pos = input_buff;
}

bool
notinput (const char *str)
{
  while (*str)
    if (*str++ != inputGet ())
      return true;
  return false;
}

/**
 * Returns the position of the current input pointer.  Only to be use to
 * restore the current input pointer using Input_RollBackToMark().
 */
const char *
Input_GetMark (void)
{
  return input_pos;
}


/**
 * Restore the current input pointer with a value returned from Input_GetMark().
 */
void
Input_RollBackToMark (const char *mark)
{
  assert (mark >= input_buff && mark < input_buff + sizeof (input_buff));
  input_pos = mark;
}


void
inputInit (const char *infile)
{
  if (!strcmp (infile, "-"))
    infile = NULL;
  FS_PushFilePObject (infile == NULL ? NULL : infile);
}


/**
 * Read a line from the input file into file global |workBuff|, with some
 * minimal error checking.
 *
 * Any empty line and any line starting with '#' followed by space and
 * one or more digits is discarded.  The '#' exception is to be able to
 * parse C preprocessed assembler files.
 * \return false when there is no input to be read, true otherwise.
 */
static bool
inputNextLineCore (void)
{
  if (gCurPObjP == NULL)
    return false;

  if (num_predefines)
    {
      /* Each predefine will inject the following two lines:
	   "        GBLx MyVariable"
	   "MyVariable SETx MyValue"
	 with x = 'L', 'S' or 'A'  */

      static bool toggle = false;

      const char *predefine = predefines[num_predefines - 1];

      /* Predefine.  We insert the values into the input stream before the
	 main code.  Would benefit from buffer overrun checks.  */
      if (!toggle)
	{
	  const char *type = strstr (predefine, " SET");
	  if (type && (type[4] == 'L' || type[4] == 'S' || type[4] == 'A')) 
	    sprintf (workBuff, "\tGBL%c %.*s", type[4], (int)(type - predefine), predefine);
	  else
	    {
	      error (ErrorError, "Invalid predefine '%s'", predefine);
	      *workBuff = '\0';
	    }
	}
      else
	{
	  strcpy (workBuff, predefine);
	  --num_predefines;
	}
  
      toggle = !toggle;
    }
  else
    {
      const char *curFile = FS_GetCurFileName ();
      int curLine = FS_GetCurLineNumber ();
      while (gCurPObjP->GetLine (workBuff, sizeof (workBuff)))
	{
	  if (gCurPObjP->type == POType_eFile)
	    errorLine (curFile, curLine, ErrorWarning, "No END found");
	  FS_PopPObject (false);
	  if (gCurPObjP == NULL)
	    return false;
	  curFile = FS_GetCurFileName ();
	  curLine = FS_GetCurLineNumber ();
	}
      gCurPObjP->lineNum++;
    }

  size_t l = strlen (workBuff);
  if (l >= 2)
    {
      /* Only needed to process gcc preprocessed assembler files.
	 If we start the line with a '#' followed by a space and one or
	 more digits then treat the whole line as a comment.
	 Like: # 5 "lib1funcs-aof.S"
	 FIXME: We could do better by using the digits as line number and
	 the next argument as a file reference of the file before
	 preprocessing.  */
      if (workBuff[0] == '#' && workBuff[1] == ' ')
	{
	  int i;
	  for (i = 2; workBuff[i] != '\0' && isdigit ((unsigned char)workBuff[i]); ++i)
	    /* */;
	  if (i > 2 && workBuff[i] == ' ')
	    {
	      input_buff[0] = 0;
	      return true;
	    }
	}
    }

  return true;
}

/**
 * Read one line of input into input_buff buffer.  No variable expansion is
 * done.  input_pos is reset to the beginning of the buffer.
 * \return false for failure, true for success.
 */
bool
inputNextLineNoSubst (void)
{
  if (!inputNextLineCore ())
    return false;
  
  /* printf("Line %04d: <%s> [NO EXPANSION]\n", FS_GetCurLineNumber (), workBuff); */
  strcpy (input_buff, workBuff);
  input_pos = input_buff;
  return true;
}

/**
 * Read one line of input into input_buff buffer and perform a variable
 * expansion.  input_pos is reset to the beginning of the buffer.
 * \return false for failure, true for success.
 */
bool
inputNextLine (void)
{
  if (!inputNextLineCore ())
    return false;

  /* printf("Line %04d: <%s>\n", FS_GetCurLineNumber (), workBuff); */
  bool status = inputArgSub ();
  /* printf("  -> <%s> (status %d)\n", input_buff, status); */
  input_pos = input_buff;
  return status;
}


/**
 * Perform environment variable substitution (objasm compatibility mode only).
 * \param inPP On entry, pointer to the input pointer to environment variable
 * which is '>' or control character/space limited.  On exit, the input pointer
 * will be updated reflecting the characters used.
 * \param outOffset On entry, offset in input_buff buffer.  On exit, offset
 * will be updated reflecting the written charactes in input_buff.
 *
 * Buffer overflow will be deteced by the caller when not all the input has
 * been consumed together with *outOffsetP == sizeof (input_buff).
 */
static void
inputEnvSub (const char **inPP, size_t *outOffsetP)
{
  /* Find end of variable.  */
  const char *inP = *inPP;
  while (*inP != '\0' && *inP != '>' && *inP > 32)
    ++inP;
  if (*inP != '>' || inP == *inPP)
    {
      /* Not a variable, had a lone '<' or "<>".  Only copy '<' to buffer.
	 Note that <> is an extension which means the same as !=.  */
      if (*outOffsetP < sizeof (input_buff))
	input_buff[(*outOffsetP)++] = '<';
      return;
    }

  /* Clone variable name into a temporary buffer.  */
  char *temp = alloca (inP - *inPP + 1);
  memcpy (temp, *inPP, inP - *inPP);
  temp[inP - *inPP] = '\0';
  char *env = getenv (temp);
  if (env == NULL)
    {
      /* No such variable defined. Warn, though we may want to error.  */
      error (ErrorWarning, "Unknown environment variable '%s'", temp);
      if (*outOffsetP < sizeof (input_buff))
	input_buff[(*outOffsetP)++] = '<';
      return;
    }

  /* Substitute variable's value, providing it won't truncate.  */
  size_t len = strlen (env);
  if (*outOffsetP + len >= MAX_LINE)
    len = MAX_LINE - *outOffsetP;
  memcpy(input_buff + *outOffsetP, env, len);
  *outOffsetP += len;

  *inPP = inP + 1;
}


/**
 * Perform variable substitution.
 * \param inPP On entry, pointer to the input pointer to first input character
 * after the $.
 * On exit, the input pointer will be updated reflecting the characters used.
 * \param outOffset On entry, offset in input_buff buffer.  On exit, offset
 * will be updated reflecting the written charactes in input_buff.
 * \param inString Flags whether the variable we're processing is in a string
 * literal.
 *
 * Buffer overflow will be deteced by the caller when not all the input has
 * been consumed together with *outOffsetP == sizeof (input_buff).
 *
 * Corrupts input_pos.
 */
static void
inputVarSub (const char **inPP, size_t *outOffsetP, bool inString)
{
  const char *inP = *inPP;

  /* $$ -> $.  */
  if (*inP == '$')
    {
      if (*outOffsetP < sizeof (input_buff))
	{
	  input_buff[(*outOffsetP)++] = '$';
	  ++(*inPP);
	}
      return;
    }

  /* Replace symbol by its definition.  */
  input_pos = inP;
  Lex label = lexGetIdNoError ();
  if (label.tag != LexId)
    {
      if (!inString)
	error (ErrorWarning, "Non-ID in $ expansion");
      else if (option_pedantic)
	error (ErrorWarning, "No $ expansion - did you perhaps mean $$");
      if (*outOffsetP < sizeof (input_buff))
	input_buff[(*outOffsetP)++] = '$';
      return;
    }

  Symbol *sym = symbolFind (&label);
  if (sym)
    {
      char buf[32];
      const char *toCopy;
      size_t toCopyLen;
      switch (sym->value.Tag)
	{
	  case ValueInt:
	    toCopyLen = sprintf (buf, "%.8X", sym->value.Data.Int.i);
	    toCopy = buf;
	    break;
	  case ValueFloat:
	    toCopyLen = sprintf (buf, "%f", sym->value.Data.Float.f);
	    toCopy = buf;
	    break;
	  case ValueString:
	    toCopyLen = sym->value.Data.String.len;
	    toCopy = sym->value.Data.String.s;
	    break;
	  case ValueBool:
	    toCopyLen = 1;
	    toCopy = (sym->value.Data.Bool.b) ? "T" : "F";
	    break;
	  default:
	    if (!inString)
	      {
		error (ErrorError, "$ expansion of '%.*s' can't be done",
		       (int)label.Data.Id.len, label.Data.Id.str);
		return;
	      }
	    toCopyLen = 0;
	    toCopy = NULL;
	    break;
	}
      if (toCopy)
	{
	  if (*outOffsetP + toCopyLen >= MAX_LINE)
	    toCopyLen = MAX_LINE - *outOffsetP;
	  memcpy (&input_buff[*outOffsetP], toCopy, toCopyLen);
	  *outOffsetP += toCopyLen;

	  /* Update our input ptr with what we have successfully consumed.  */
	  inP = input_pos;
	  /* Skip any . after the id - it indicates concatenation (e.g.
	     $foo.bar).  */
	  if (*inP == '.')
	    ++inP;
	  *inPP = inP;
	  return;
	}
    }
  if (!inString)
    {
      /* Not in string literal, so this is an error.  */
      error (ErrorError, "Unknown variable '%.*s' for $ expansion",
	     (int)label.Data.Id.len, label.Data.Id.str);
    }
  else if (option_pedantic)
    error (ErrorWarning, "No $ expansion as variable '%.*s' is not defined",
	   (int)label.Data.Id.len, label.Data.Id.str);
  if (*outOffsetP < sizeof (input_buff))
    input_buff[(*outOffsetP)++] = '$';
}

/**
 * Copy the line from |workBuff| to |input_buff|, while performing any
 * substitutions.
 * \returns true if successful
 */
static bool
inputArgSub (void)
{
  size_t outOffset = 0;
  const char *inP = workBuff;

  /* Process all characters in the line.  */
  while (*inP && outOffset < sizeof (input_buff))
    {
      /* Copy each input character, until a special symbol is found.  */
      while (*inP
	     && *inP != '"' && *inP != '\''
	     && *inP != '|' && *inP != '$'
	     && *inP != ';' && *inP != '<'
	     && outOffset < sizeof (input_buff))
	input_buff[outOffset++] = *inP++;

      /* Process special characters.  */
      char c;
      switch (c = *inP)
	{
	  case ';':
	    { /* Comment follows; just copy it all.  */
	      size_t len = strlen (inP) + 1;
	      if (outOffset + len >= MAX_LINE)
		len = MAX_LINE - outOffset - len;
	      memcpy (input_buff + outOffset, inP, len);
	      outOffset += len;
	      inP += len;
	    }
	    break;

	  case '<': /* Characters enclosed between <...> in ObjAsm mode.  */
	    ++inP;
	    inputEnvSub (&inP, &outOffset);
	    break;

	  case '|': /* Copy "|xxx|" as is.  */
	    if (outOffset < sizeof (input_buff))
	      input_buff[outOffset++] = *inP++;
	    while (outOffset < sizeof (input_buff) && *inP)
	      {
		input_buff[outOffset++] = *inP++;
		if (*inP == c)
		  break;
	      }
	    /* We don't check on unmatched |.  */
	    break;

	  case '\'':
	  case '\"':
	    /* String: variable substitution is warning-less when it it
	       fails.  */
	    if (outOffset < sizeof (input_buff))
	      input_buff[outOffset++] = *inP++;
	    while (outOffset < sizeof (input_buff) && *inP)
	      {
		char cc = *inP++;
		if (cc == '$')
		  inputVarSub (&inP, &outOffset, true);
		else if (cc == '<')
		  inputEnvSub (&inP, &outOffset);
		else
		  {
		    input_buff[outOffset++] = cc;
		    if (cc == c)
		      break;
		  }
	      }
	    /* We don't check on unmatched '/".  */
	    break;

	  case '$': /* Do variable substitution - $ */
	    ++inP;
	    inputVarSub (&inP, &outOffset, false);
	    break;
	}
    }
  if (*inP)
    {
      /* There are still unprocessed characters, so we overflowed.  */
      errorAbort ("Line expansion resulted in overflow - line ignored");
      input_buff[0] = 0;
      return false;
    }

  input_buff[outOffset] = 0;
  return true;
}

/**
 * Try to read character c from input followed (optionally) by zero or more
 * space characters.
 * \param c Character which is expected to be in input stream. If so, discard
 * it.
 * \param spacesToo Try to discard zero or more spaces as well.
 * \return true when character c was the next character in the input stream,
 * false otherwise.
 */
bool
Input_Match (char c, bool spacesToo)
{
  if (c == '\0' || *input_pos != c)
    return false;
  ++input_pos;
  if (spacesToo)
    while (isspace ((unsigned char)*input_pos))
      ++input_pos;
  return true;
}


/**
 * Try to read the keyword followed by space, NUL or start comment character.
 * \param keyword Keyword to match
 * \return true when keyword got matched, the corresponding input characters
 * get consumed. Otherwise, false, and no input characters get consumed.
 */
bool
Input_MatchKeyword (const char *keyword)
{
  int matched = 0;
  while (input_pos[matched] && input_pos[matched] == keyword[matched])
    ++matched;
  if (keyword[matched] == '\0'
      && (isspace ((unsigned char)input_pos[matched])
          || input_pos[matched] == ';'
          || input_pos[matched] == '\0'))
    {
      input_pos += matched;
      return true;
    }
  return false;
}

bool
Input_MatchKeywordLower (const char *keyword)
{
  int matched = 0;
  while (input_pos[matched]
	 && tolower ((unsigned char)input_pos[matched]) == keyword[matched])
    ++matched;
  if (keyword[matched] == '\0'
      && (isspace ((unsigned char)input_pos[matched])
          || input_pos[matched] == ';'
          || input_pos[matched] == '\0'))
    {
      input_pos += matched;
      return true;
    }
  return false;
}


/**
 * Try to read a symbol.
 * \return NULL on error, otherwise points to begin of symbol and symbol length
 * is *ilen bytes.
 */
const char *
Input_Symbol (size_t *ilen)
{
  const char *rslt;
  size_t len;
  if (*input_pos == '|')
    {
      /* Symbol is bracketed between two '|'.  */
      rslt = ++input_pos;
      for (len = 0; input_pos[len] != '\0' && input_pos[len] != '|'; ++len)
	/* */;
      if (input_pos[len] == '\0')
	{
	  error (ErrorError, "Failed to read symbol (forgot second '|' ?)");
	  *ilen = 0;
	  return NULL;
	}
      ++input_pos; /* Skip second '|'.  */
    }
  else
    {
      /* Symbol needs to start with a letter (upper- or lowercase), followed by
	 letter, digits and underscore.
	 We do allow an underscore as starting character as well.  */
      rslt = input_pos;
      for (len = 0; input_pos[len] != '\0'; ++len)
	{
          if (len == 0)
	    {
	      if (!isalpha ((unsigned char)input_pos[len])
	          && input_pos[len] != '_')
	        break;
	    }
          else
	    {
	      if (!isalnum ((unsigned char)input_pos[len])
	          && !isdigit ((unsigned char)input_pos[len])
	          && input_pos[len] != '_')
	        break;
	    }
        }
    }
  *ilen = len;
  input_pos += len;
  return len ? rslt : NULL;
}


/**
 * Reads a string (supporting escape characters etc).
 * \param len Will contain the length of the returned string.  As a string
 * can contain NUL characters, you really need to use this length parameter
 * instead of calculating the length yourself using strlen().
 * \return ptr to malloc block holding the string contents (caller gets
 * ownership).
 */
char *
Input_GetString (size_t *len)
{
  size_t curLen = 0;
  size_t maxLen = 8;
  char *result = malloc (maxLen);
  if (result == NULL)
    errorOutOfMem ();

  while (1)
    {
      int c = (unsigned char) inputGet ();
      if (c == '\0')
	{
	  /* End of line found without terminating '"', return whatever we
	     have right now.  */
	  error (ErrorError, "String continues over newline");
	  break;
	}
      if (c == '"')
	{
	  if (inputLook () != '"')
	    break; /* End of string found.  */
	  inputSkip ();
	}
      if (c == '\\')
	{
	  c = (unsigned char) inputGet ();
	  switch (c)
	    {
	      case '0':
	      case '1':
	      case '2':
	      case '3':
	      case '4':
	      case '5':
	      case '6':
	      case '7':
		{
		  /* Octal.  */
		  c -= '0';
		  int i = (unsigned char) inputLook ();
		  if (i >= '0' && i <= '7')
		    {
		      c = 8*c + i - '0';
		      inputSkip ();
		      i = (unsigned char) inputLook ();
		      if (i >= '0' && i <= '7')
			{
			  c = 8*c + i - '0';
			  inputSkip ();
			}
		    }
		  break;
		}

	      case 'x':
		{
		  /* Hex.  */
		  int i = (unsigned char) inputLook ();
		  if (!isxdigit (i))
		    {
		      error (ErrorWarning, "Not a hex escape sequence");
		      inputUnGet ('x');
		    }
		  else
		    {
		      i |= 0x20;
		      c = i >= 'a' ? i - 'a' + 10 : i - '0';
		      inputSkip ();
		      i = (unsigned char) inputLook ();
		      if (!isxdigit (i))
			error (ErrorWarning, "Not a hex escape sequence");
		      else
			{
			  i |= 0x20;
			  c = 16*c + (i >= 'a' ? i - 'a' + 10 : i - '0');
			  inputSkip ();
			}
		    }
		  break;
		}

	      case 'a':
		c = 7;
		break;

	      case 'b':
		c = 8;
		break;

	      case 'f':
		c = 12;
		break;

	      case 'n':
		c = 10;
		break;

	      case 'r':
		c = 13;
		break;

	      case 't':
		c = 9;
		break;

	      case 'v':
		c = 11;
		break;

	      case '\\':
		c = '\\';
		break;

	      case '\"':
		c = '\"';
		break;

	      default:
		error (ErrorWarning, "Unknown escape sequence");
		break;
	    }
	}
      /* Add c to string result.
         Make sure there is place for at least two characters so we can
         always safely add a terminating NUL character without doing an
         additional check.  */
      if (curLen + 1 == maxLen)
	{
	  maxLen *= 2;
	  result = realloc (result, maxLen);
	  if (result == NULL)
	    errorOutOfMem ();
	}
      result[curLen++] = c;
    }
  result[curLen] = '\0';
  *len = curLen;
  
  return result;
}


const char *
inputSymbol (size_t *ilen, char del)
{
  const char *p = input_pos;

  if (del)
    {
      int c;
      while ((c = (unsigned char)*p) != 0 && c != del)
	p++;
    }
  else
    {
      int c;
      while ((c = (unsigned char)*p) != 0
	     && (isalnum (c) || c == '_'))
	p++;
    }
  *ilen = p - input_pos;
  input_pos = p;
  return input_pos - *ilen;
}


void
inputThisInstead (const char *p)
{
  strcpy (input_buff, p);
  input_pos = input_buff;
}

void
Input_ShowLine (void)
{
  /* When there is nothing on the filestack, we've done parsing the input
     files so it does not make sense to mark (the last) input line for errors
     happening later on.  */
  if (gCurPObjP == NULL || *input_pos == '\0')
    return;
  ptrdiff_t posNoTAB = input_pos - input_buff;
  size_t posReal, len;
  for (posReal = 0, len = 0; input_buff[len]; ++len)
    {
      if (posNoTAB)
	{
	  if (input_buff[len] == '\t')
	    posReal = (posReal + 8) & -8;
	  else
	    ++posReal;
	  --posNoTAB;
	}
      putchar (input_buff[len]);
    }
  putchar ('\n');
  while (posReal--)
    putchar ('-');
  puts ("^");
}

size_t
Input_GetColumn (void)
{
  if (gCurPObjP == NULL)
    return 0;
  return input_pos - input_buff;
}
