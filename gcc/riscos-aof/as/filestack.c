/*
 * filestack.c
 *
 * (c) Andy Duplain, August 1992.
 *     Added line numbers  Niklas Röjemo
 *     Added filenames     Darren Salt
 */
#include "sdk-config.h"
#include <stdio.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "input.h"
#include "error.h"
#include "whileif.h"

#define STACKSIZE  10

typedef struct FileStack
  {
    FILE *file;
    const char *name;
    long int line, no;
    int if_depth;
    WhileBlock *whilestack;
  }
FileStack;

static FileStack stack[STACKSIZE];
static int top = 0;

long int fileCurrentNo;

int 
push_file (FILE * fp)
{
  static long int fileNo = 0;
  if (top == STACKSIZE)
    {
      error (ErrorSerious, TRUE, "Maximum file nesting level reached (%d)", STACKSIZE);
      return (-1);
    }
  stack[top].line = inputLineNo;
  stack[top].name = inputName;
  stack[top].no = fileCurrentNo;
  stack[top].if_depth = if_depth;
  stack[top].whilestack = whileCurrent;
  stack[top++].file = fp;
  whileCurrent = 0;
  if_depth = 0;
  fileCurrentNo = fileNo++;
  return 0;
}

FILE *
pop_file (void)
{
  testUnmatched ();
  if (top)
    {
      inputLineNo = stack[--top].line;
      inputName = (char *) stack[top].name;
      fileCurrentNo = stack[top].no;
      whileCurrent = stack[top].whilestack;
      if_depth = stack[top].if_depth;
      return stack[top].file;
    }
  return NULL;
}

const int 
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
