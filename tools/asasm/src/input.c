/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2014 GCCSDK Developers
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */

#include "config.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "common.h"
#include "error.h"
#include "filestack.h"
#include "input.h"
#include "lex.h"
#include "macros.h"
#include "main.h"
#include "symbol.h"

#define MAX_LINE (4096)

static char input_buff[MAX_LINE + 256];
static const char *input_pos; /* Ptr inside input_buff.  Can be NULL when input_buff is being filled up.  */
static char workBuff[MAX_LINE + 1]; /* holds each line from input file */

#define MAX_PREDEFINES 10
static const char *oPreDefines[MAX_PREDEFINES];
static unsigned oPreDefCnt;
static unsigned oPreDefIdx;

static bool Input_ArgSub (bool warnOnVarSubFail);

void
Input_PrepareForPhase (Phase_e phase)
{
  switch (phase)
    {
      case ePassTwo:
	/* We need to re-parse the predefined variables.  */
	oPreDefIdx = 0;
	break;

      default:
	break;
    }
}


bool
Input_AddPredefine (const char *preDef)
{
  if (oPreDefCnt == MAX_PREDEFINES)
    return true;
  oPreDefines[oPreDefCnt++] = preDef;
  return false;
}


char
Input_Look (void)
{
  return *input_pos;
}


char
Input_LookLower (void)
{
  return tolower ((unsigned char)*input_pos);
}

/**
 * \return true if next input character is NUL (i.e. EOL) or start of a comment.
 */
bool
Input_IsEolOrCommentStart (void)
{
  const char c = *input_pos;
  return c == '\0' || c == ';' || (c == '*' && input_pos == input_buff);
}


char
Input_LookN (int n)		/* Unsafe */
{
  return input_pos[n];
}


char
Input_LookNLower (int n)		/* Unsafe */
{
  return tolower ((unsigned char)input_pos[n]);
}

char
Input_GetC (void)
{
  return (*input_pos) ? *input_pos++ : *input_pos;
}


char
Input_GetCLower (void)
{
  char c = *input_pos ? *input_pos++ : *input_pos;
  return tolower ((unsigned char)c);
}


/**
 * Undo the last character got from input buffer.
 */
void
Input_UnGetC (char c)
{
  if (input_pos > input_buff && input_pos[-1] == c)
    input_pos--;
  else
    assert (*input_pos == '\0' && c == '\0' && "illegal character");
}


void
Input_Skip (void)
{
  if (*input_pos)
    input_pos++;
}


void
Input_SkipN (size_t n)
{
  while (*input_pos && n--)
    input_pos++;
  assert (*input_pos || n == 0);
}


/**
 * Returns the rest of the line of the current input and consumes it.
 */
const char *
Input_Rest (void)
{
  const char * const t = input_pos;
  if (t)
    while (*input_pos)
      ++input_pos;
  return t;
}

#if DEBUG
const char *
Input_Line (void)
{
  return input_buff;
}
#endif


void
Input_SkipWS (void)
{
  const char *p = input_pos;
  while (*p && isspace ((unsigned char)*p))
    p++;
  input_pos = p;
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
Input_NextLineCore (void)
{
  if (oPreDefIdx != oPreDefCnt)
    {
      /* Each predefine will inject the following two lines:
	   "        GBLx MyVariable" (toggle == false)
	   "MyVariable SETx MyValue" (toggle == true)
	 with x = 'L', 'S' or 'A'  */

      static bool toggle = false;

      const char *predefine = oPreDefines[oPreDefIdx];

      /* Predefine.  We insert the values into the input stream before the
	 main code.  Would benefit from buffer overrun checks.  */
      if (!toggle)
	{
	  const char *type = strstr (predefine, " SET");
	  if (type && (type[4] == 'L' || type[4] == 'S' || type[4] == 'A'))
	    {
	      int len = snprintf (workBuff, sizeof (workBuff), "\tGBL%c %.*s", type[4], (int)(type - predefine), predefine);
	      if ((size_t)len >= sizeof (workBuff))
		{
		  Error (ErrorError, "Failed to set predefine '%s'", predefine);
		  *workBuff = '\0';
		}
	    }
	  else
	    {
	      Error (ErrorError, "Invalid predefine '%s'", predefine);
	      *workBuff = '\0';
	    }
	}
      else
	{
	  strcpy (workBuff, predefine);
	  ++oPreDefIdx;
	}
  
      toggle = !toggle;
    }
  else
    {
      if (gCurPObjP == NULL || gCurPObjP->GetLine (workBuff, sizeof (workBuff)))
	return false;
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
 * Read one line of input into input_buff buffer and perform a variable
 * expansion.  input_pos is reset to the beginning of the buffer.
 * \return false for failure, true for success.
 */
bool
Input_NextLine (Level_e level)
{
  input_pos = NULL; /* Disable Input_ShowLine().  */

  if (!Input_NextLineCore ())
    return false;

  bool status;
  if (level == eNoVarSubst)
    {
      strcpy (input_buff, workBuff);
      status = true;
    }
  else
    status = Input_ArgSub (level == eVarSubst);

  input_pos = input_buff;
  return status;
}


/**
 * Perform variable substitution.
 * \param inPP On entry, pointer to the input pointer to '$'.
 * On exit, the input pointer will be updated reflecting the characters used.
 * \param outOffset On entry, offset in input_buff buffer.  On exit, offset
 * will be updated reflecting the written charactes in input_buff.
 * \param inString Flags whether the variable we're processing is in a string
 * literal.
 * \return false when produced output needs environment variable expansion.
 * true when this may not be done.
 *
 * Buffer overflow will be detected by the caller when not all the input has
 * been consumed together with *outOffsetP == sizeof (input_buff).
 *
 * Temporarily changes input_pos.
 */
static bool
Input_VarSub (const char **inPP, size_t *outOffsetP, bool inString, bool warnOnVarSubFail)
{
  const char *inP = *inPP;

  assert (*inP == '$');

  /* $$ -> $.  */
  if (*++inP == '$')
    {
      *inPP += 2;
      if (*outOffsetP < sizeof (input_buff))
	input_buff[(*outOffsetP)++] = '$';
      return true;
    }

  /* Replace symbol by its definition.  */
  assert (input_pos == NULL);
  input_pos = inP;
  const Lex label = Lex_GetIDNoError ();
  input_pos = NULL;
  if (label.tag != LexId)
    {
      if (warnOnVarSubFail)
	{
	  if (!inString)
	    Error (ErrorWarning, "Non-ID in $ expansion");
	  else if (option_pedantic)
	    Error (ErrorWarning, "No $ expansion - did you perhaps mean $$");
	}
    }
  else
    {
      Symbol *symP = Symbol_Find (label.Data.Id.str, label.Data.Id.len);
      if (symP && (symP->attr.type & SYMBOL_RW) != 0)
	{
	  char buf[32];
	  const char *toCopy;
	  size_t toCopyLen;
	  switch (symP->attr.value.Tag)
	    {
	      case ValueInt:
		toCopyLen = sprintf (buf, "%.8X", symP->attr.value.Data.Int.i);
		toCopy = buf;
		break;
	      case ValueString:
		toCopyLen = symP->attr.value.Data.String.len;
		toCopy = symP->attr.value.Data.String.s;
		assert (symP->attr.value.Data.String.s || symP->attr.value.Data.String.len == 0);
		if (toCopy == NULL)
		  toCopy = "";
		break;
	      case ValueBool:
		toCopyLen = 1;
		toCopy = (symP->attr.value.Data.Bool.b) ? "T" : "F";
		break;
	      default:
		/* Only GBL{L,S,A} and LCL{L,S,A} variables are used for
		   substitution.  */
		assert (0);
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
	      inP += label.Data.Id.len;
	      /* Skip any . after the id - it indicates concatenation (e.g.
		 $foo.bar).  */
	      if (*inP == '.')
		++inP;
	      *inPP = inP;
	      return false;
	    }
	}
      if (warnOnVarSubFail)
	{
	  if (!inString)
	    Error (ErrorWarning, "Unknown variable '%.*s' for $ expansion, you want to use vertical bars ?",
		   (int)label.Data.Id.len, label.Data.Id.str);
	  else if (option_pedantic)
	    Error (ErrorWarning, "No $ expansion as variable '%.*s' is not defined, you want to use double $ ?",
		   (int)label.Data.Id.len, label.Data.Id.str);
	}
    }
  if (*outOffsetP < sizeof (input_buff))
    input_buff[(*outOffsetP)++] = '$';
  *inPP = inP;
  return true;
}

/**
 * Copy the line from |workBuff| to |input_buff|, while performing any
 * substitutions.
 * \returns true if successful
 */
static bool
Input_ArgSub (bool warnOnVarSubFail)
{
  size_t outOffset = 0;
  const char *inP = workBuff;

  /* Process all characters in the line.  */
  while (*inP && outOffset < sizeof (input_buff))
    {
      switch (*inP)
	{
	  case ';':
	    { /* Comment follows; just copy it all.  */
	      size_t len = strlen (inP);
	      if (outOffset + len >= MAX_LINE)
		len = MAX_LINE - outOffset - len;
	      memcpy (input_buff + outOffset, inP, len);
	      outOffset += len;
	      inP += len;
	      break;
	    }

	  case '|': /* Copy "|xxx|" as is.  */
	    {
	      if (outOffset < sizeof (input_buff))
		input_buff[outOffset++] = *inP++;
	      while (outOffset < sizeof (input_buff) && *inP)
		{
		  input_buff[outOffset++] = *inP++;
		  if (inP[-1] == '|')
		    break;
		}
	      /* We don't check on unmatched |.  */
	      break;
	    }

	  case '\'':
	  case '\"':
	    {
	      /* String: variable substitution is warning-less when it it
		 fails.  */
	      char c = *inP;
	      if (outOffset < sizeof (input_buff))
		input_buff[outOffset++] = *inP++;
	      bool disableVarSubst = false;
	      while (outOffset < sizeof (input_buff) && *inP)
		{
		  if (*inP == '$' && !disableVarSubst)
		    Input_VarSub (&inP, &outOffset, true, warnOnVarSubFail);
		  else
		    {
		      char cc = *inP++;
		      input_buff[outOffset++] = cc;
		      if (cc == '|')
			disableVarSubst = !disableVarSubst;
		      if (cc == c)
			break;
		    }
		}
	      /* We don't check on unmatched ' or ".  */
	      break;
	    }

	  case '$': /* Do variable substitution - $ */
	    {
	      const size_t origOutOffset = outOffset;
	      bool dontReExpand = Input_VarSub (&inP, &outOffset, false, warnOnVarSubFail);
	      size_t expandedLen = outOffset - origOutOffset;
	      if (expandedLen && !dontReExpand)
		{
		  size_t remainingInput = strlen (inP) + 1;
		  if (expandedLen + remainingInput <= sizeof (workBuff))
		    {
		      /* Reprocess the expanded variable as input data.  */
		      memmove (workBuff + expandedLen, inP, remainingInput);
		      memcpy (workBuff, input_buff + origOutOffset, expandedLen);
		      outOffset = origOutOffset;
		      inP = workBuff;
		    }
		  else
		    {
		      /* Would overflow workBuff.  */
		      Error_Abort ("Line expansion resulted in overflow - line ignored");
		      input_buff[0] = 0;
		      return false;
		    }
		}
	      break;
	    }

	  default:
	    input_buff[outOffset++] = *inP++;
	    break;
	}
    }
  if (*inP)
    {
      /* There are still unprocessed characters, so we overflowed.  */
      Error_Abort ("Line expansion resulted in overflow - line ignored");
      input_buff[0] = 0;
      return false;
    }

  if (outOffset < sizeof (input_buff))
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
 * See also Input_MatchKeywordLower().
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


/**
 * Try to read the keyword case insensitive followed by space, NUL or start
 * comment character.
 * \param keyword Keyword to match, needs to be lowercase.
 * \return true when keyword got matched, the corresponding input characters
 * get consumed. Otherwise, false, and no input characters get consumed.
 * See also Input_MatchKeyword().
 */
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


bool
Input_MatchString (const char *str)
{
  int matched = 0;
  while (input_pos[matched] && input_pos[matched] == str[matched])
    ++matched;
  if (str[matched] == '\0')
    {
      input_pos += matched;
      return true;
    }
  return false;
}


bool
Input_MatchStringLower (const char *str)
{
  int matched = 0;
  while (input_pos[matched]
         && tolower ((unsigned char)input_pos[matched]) == str[matched])
    ++matched;
  if (str[matched] == '\0')
    {
      input_pos += matched;
      return true;
    }
  return false;
}


/**
 * Checks if the end of the current keyword has been reached.
 */
bool
Input_IsEndOfKeywordN (size_t n)
{
  unsigned char c = input_pos[n];
  return c == '\0' || isspace (c) || c == ';';
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
      /* Symbol is bracketed between two '|'.  Support double '||' bracketing
         as well.  */
      bool isDbl;
      if (input_pos[1] == '|')
	{
	  ++input_pos;
	  isDbl = true;
	}
      else
	isDbl = false;
      rslt = ++input_pos;
      for (len = 0;
	   input_pos[len] != '\0' && !(input_pos[len] == '|' && (!isDbl || input_pos[len + 1] == '|'));
	   ++len)
	/* */;
      if (input_pos[len] == '\0')
	{
	  Error (ErrorError, "Failed to read symbol (forgot second '%s' ?)", isDbl ? "||" : "|");
	  *ilen = 0;
	  return NULL;
	}
      if (isDbl)
	input_pos += 2; /* Skip second '||'  */
      else
        input_pos += 1; /* Skip second '|'.  */
    }
  else
    {
      /* Symbol needs to start with a letter (upper- or lowercase) or
	 underscore, followed by letter, digits and underscore.  */
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
    Error_OutOfMem ();

  while (1)
    {
      unsigned char c = Input_GetC ();
      if (c == '\0')
	{
	  /* End of line found without terminating '"', return whatever we
	     have right now.  */
	  Error (ErrorError, "String continues over newline");
	  break;
	}
      if (c == '"')
	{
	  if (Input_Look () != '"')
	    break; /* End of string found.  */
	  Input_Skip ();
	}
      if (c == '\\')
	{
	  c = Input_GetC ();
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
		  unsigned char i = Input_Look ();
		  if (i >= '0' && i <= '7')
		    {
		      c = 8*c + i - '0';
		      Input_Skip ();
		      i = Input_Look ();
		      if (i >= '0' && i <= '7')
			{
			  c = 8*c + i - '0';
			  Input_Skip ();
			}
		    }
		  break;
		}

	      case 'x':
		{
		  /* Hex.  */
		  unsigned char i = Input_Look ();
		  if (!isxdigit (i))
		    {
		      Error (ErrorWarning, "Not a hex escape sequence");
		      Input_UnGetC ('x');
		    }
		  else
		    {
		      unsigned intResult = 0;
		      bool gaveWarning = false;
		      do
			{
			  i |= 0x20;
			  intResult = 16*intResult + (i >= 'a' ? i - 'a' + 10 : i - '0');
			  if (intResult >= 0x100 && !gaveWarning)
			    {
			      gaveWarning = true;
			      Error (ErrorWarning, "Hex escape sequence out of range");
			    }
			  Input_Skip ();
			  i = Input_Look ();
			} while (isxdigit (i));
		      c = intResult & 0xFF;
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
	      case '\"':
	      case '\?':
	      case '\'':
		/* Nothing to do.  */
		break;

	      default:
		Error (ErrorWarning, "Unknown escape sequence");
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
	    Error_OutOfMem ();
	}
      result[curLen++] = c;
    }
  result[curLen] = '\0';
  *len = curLen;
  
  return result;
}


const char *
Input_Symbol2 (size_t *ilen, char del)
{
  const char *p = input_pos;

  if (del)
    {
      unsigned char c;
      while ((c = *p) != 0 && c != del && c != ';')
	p++;
    }
  else
    {
      unsigned char c;
      while ((c = *p) != 0 && (isalnum (c) || c == '_'))
	p++;
    }
  *ilen = p - input_pos;
  input_pos = p;
  return input_pos - *ilen;
}

void
Input_ThisInstead (const char *p)
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
  if (gCurPObjP == NULL || input_pos == NULL || *input_pos == '\0')
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
      putc (input_buff[len], stderr);
    }
  putc ('\n', stderr);
  while (posReal--)
    putc ('-', stderr);
  fputs ("^\n", stderr);
}

size_t
Input_GetColumn (void)
{
  if (gCurPObjP == NULL || input_pos == NULL)
    return 0;
  return input_pos - input_buff;
}
