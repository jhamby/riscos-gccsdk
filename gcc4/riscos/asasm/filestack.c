/*
 * AS an assembler for ARM
 * Copyright (c) Andy Duplain, August 1992.
 *     Added line numbers  Niklas Röjemo
 *     Added filenames     Darren Salt
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
 * filestack.c
 */

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#include <string.h>

#include "error.h"
#include "filestack.h"
#include "input.h"
#include "whileif.h"

#define STACKSIZE  (10)

typedef struct FileStack
  {
    FILE *file;
    char *name;
    long int line, no;
    int if_depth;
    WhileBlock *whilestack;
  }
FileStack;

static FileStack stack[STACKSIZE];
static int top = 0;

int
push_file (FILE * fp)
{
  if (top == STACKSIZE)
    {
      error (ErrorSerious, TRUE, "Maximum file nesting level reached (%d)", STACKSIZE);
      return -1;
    }
  stack[top].line = inputLineNo;
  /* Need to duplicate this, in case it's destroyed under us */
  stack[top].name = strdup(inputName);
  if (stack[top].name == NULL)
    {
      error (ErrorSerious, TRUE, "No space for filename string");
      return -1;
    }
  stack[top].if_depth = if_depth;
  stack[top].whilestack = whileCurrent;
  stack[top++].file = fp;
  whileCurrent = 0;
  if_depth = 0;
  return 0;
}

FILE *
pop_file (void)
{
  testUnmatched ();
  if (top)
    {
      inputLineNo = stack[--top].line;
      if (inputName != NULL)
	free((void *)inputName);
      inputName = stack[top].name;
      whileCurrent = stack[top].whilestack;
      if_depth = stack[top].if_depth;
      return stack[top].file;
    }
  return NULL;
}

int
get_file (const char **file, long int *line)
{
  static int f;

  if (!file)
    {
      f = top;
    }
  else
    {
      *file = stack[f].name;
      *line = stack[f].line;
    }
  return f-- >= 0;
}
