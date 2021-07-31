#include <stdlib.h>

#include "test_data_part1.h"

char *char_ptr = "char ptr: ok";
const char *const_char_ptr = "const char ptr: ok";
const char * const const_char_const_ptr = "const char ptr const: ok";

char char_array[] = "char array: ok";
const char const_char_array[] = "const char array: ok";

int an_int = 42;
const int a_const_int = 2*42;

const char *
func1 (void)
{
  return "function ptr 1 called: ok";
}

const char *
func2 (void)
{
  return "function ptr 2 called: ok";
}

const char *(*func1_ptr)(void) = func1;
const char *(* const const_func2_ptr)(void) = func2;

const char *(*func1_array[2])(void) = { NULL, func1 };
const char *(*const const_func2_array[2])(void) = { NULL, func2 };
