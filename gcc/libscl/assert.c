/* Support for __assert2 in libscl
   This file is part of GCC's SharedCLibrary stubs
   Written by Peter Naulls <peter@chocky.org> */


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void __assert2(const char *expression, const char *function,
               const char *file, int lineno) {

  fprintf(stderr, "%s:%u: failed assertion '%s' in function '%s'\n",
                   file, lineno, expression, function);
  abort();
}



