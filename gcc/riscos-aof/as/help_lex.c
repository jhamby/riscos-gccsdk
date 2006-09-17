/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas R�emo
 * Copyright (c) 2004-2006 GCCSDK Developers
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
 * help_lex.c
 */

#include <ctype.h>

#include "error.h"
#include "global.h"
#include "help_lex.h"
#include "main.h"

static int
char2digit (char c)
{
  if (c > 'a')
    return c + 10 - 'a';
  if (c > 'A')
    return c + 10 - 'A';
  return c - '0';
}

WORD
lexGetCharFromString (int *ilen, const char **istr)
{
  const char *str = *istr;
  int len = *ilen;
  char c;

  if (*str == '\\' && !objasm)
    {				/* An escape character */
      if (--len < 1)
	{
	  error (ErrorError, TRUE, "No character after '\\'");
	  c = 0;
	  str++;
	}
      else
	{
	  switch (*++str)
	    {			/* Is it one of the special characters ? */
	    case 'a':
	      str++;
	      len--;
	      c = '\a';
	      break;
	    case 'b':
	      str++;
	      len--;
	      c = '\b';
	      break;
	    case 'f':
	      str++;
	      len--;
	      c = '\f';
	      break;
	    case 'n':
	      str++;
	      len--;
	      c = '\n';
	      break;
	    case 'r':
	      str++;
	      len--;
	      c = '\r';
	      break;
	    case 't':
	      str++;
	      len--;
	      c = '\t';
	      break;
	    case 'v':
	      str++;
	      len--;
	      c = '\v';
	      break;
	    case '\'':
	      str++;
	      len--;
	      c = '\'';
	      break;
	    case '"':
	      str++;
	      len--;
	      c = '"';
	      break;
	    case '\\':
	      str++;
	      len--;
	      c = '\\';
	      break;
	    case 'x':
	      str++;
	      len--;
	      switch (len)
		{		/* It is an hex character */
		case 0:
		case 1:
		  error (ErrorError, TRUE, "Not two hex digits after \\x");
		  str += len;
		  len = 0;
		  c = 0;
		  break;
		default:
		  if (isxdigit (str[0]) && isxdigit (str[1]))
		    c = char2digit (str[0]) * 16 + char2digit (str[1]);
		  else
		    {
		      error (ErrorError, TRUE, "Illegal hex digit after \\x");
		      c = 0;
		    }
		  len -= 2;
		  str += 2;
		  break;
		}
	      break;
	    case '0':
	    case '1':
	    case '2':
	    case '3':
	    case '4':
	    case '5':
	    case '6':
	    case '7':
	      switch (len)
		{		/* It is a octal character */
		case 0:
		case 1:
		case 2:
		  error (ErrorError, TRUE, "Not three octal digits after \\");
		  str += len;
		  len = 0;
		  c = 0;
		  break;
		default:
		  if (str[0] >= '0' && str[0] <= '9' &&
		      str[1] >= '0' && str[1] <= '9' &&
		      str[2] >= '0' && str[2] <= '9')
		    c = (str[0] - '0') * 64 + (str[1] - '0') * 8 + str[2] - '0';
		  else
		    {
		      error (ErrorError, TRUE, "Illegal octal character after \\");
		      c = 0;
		    }
		  len -= 3;
		  str += 3;
		  break;
		}
	      break;
	    default:		/* Do not understand */
	      error (ErrorError, TRUE, "Illegal character after \\");
	      c = *str++;
	      break;
	    }
	}
    }
  else
    {				/* An ordinary character */
      c = *str++;
      len--;
    }
  *istr = str;
  *ilen = len;
  return c;
}

WORD
lexChar2Int (BOOL rev, int len, const char *str)
{
  char c[4];
  int i = 0;
  while (len && i < 4)
    c[i++] = lexGetCharFromString (&len, &str);
  if (rev)
    {
      switch (i)
	{
	case 0:
	  error (ErrorError, TRUE, "Empty character");
	  return 0;
	case 1:
	  return c[0];
	case 2:
	  return c[0] << 8 | c[1];
	case 3:
	  return c[0] << 16 | c[1] << 8 | c[2];
	case 4:
	  return c[0] << 24 | c[1] << 16 | c[2] << 8 | c[3];
	default:
	  error (ErrorError, TRUE, "Multi character bigger than 4 bytes (%d)", i);
	  return 0;
	}
    }
  else
    {
      switch (i)
	{
	case 0:
	  error (ErrorError, TRUE, "Empty character");
	  return 0;
	case 1:
	  return c[0];
	case 2:
	  return c[1] << 8 | c[0];
	case 3:
	  return c[2] << 16 | c[1] << 8 | c[0];
	case 4:
	  return c[3] << 24 | c[2] << 16 | c[1] << 8 | c[0];
	default:
	  error (ErrorError, TRUE, "Multi character bigger than 4 bytes (%d)", i);
	  return 0;
	}
    }
  return 0;
}
