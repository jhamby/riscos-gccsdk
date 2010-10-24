/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2004-2010 GCCSDK Developers
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

ARMWord
lexChar2Int (bool rev, int len, const char *str)
{
  ARMWord result;
  switch (len)
    {
      case 0:
	error (ErrorError, "Empty character");
	result = 0;
	break;
      case 1:
	result = str[0];
	break;
      case 2:
	result = (rev) ? (str[1] << 8) | str[0]
	               : (str[0] << 8) | str[1];
	break;
      case 3:
	result = (rev) ? (str[2] << 16) | (str[1] << 8) | str[0]
	               : (str[0] << 16) | (str[1] << 8) | str[2];
	break;
      default:
      case 4:
	result = (rev) ? (str[3] << 24) | (str[2] << 16) | (str[1] << 8) | str[0]
	               : (str[0] << 24) | (str[1] << 16) | (str[2] << 8) | str[3];
	break;
    }

  return result;
}
