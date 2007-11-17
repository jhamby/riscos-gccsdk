/* GCCSDK Module testsuite:
 *
 *   Module data can be stored as read-only date and is then part of the
 *   module code not requiring any specific relocation.  Writable module
 *   data is stored in the module RMA workspace and needs runtime
 *   relocation via the relocation data offset found on the runtime stack.
 * 
 * Written by John Tytgat  <John.Tytgat@aaug.net>
 */

#include <stdio.h>
#include <stdlib.h>

#include "MdlHdr.h"
#include "test_data_part1.h"

static char *static_char_ptr = "static char ptr: ok";
static const char *static_const_char_ptr = "static const char ptr: ok";
static const char * const static_const_char_const_ptr = "static const char ptr const: ok";

static char static_char_array[] = "static char array: ok";
static const char static_const_char_array[] = "static const char array: ok";

static int a_static_int = 3*42;
static const int a_static_const_int = 4*42;

int
main (int argc, char *argv[])
{
  puts (char_ptr); /* needs static relocation data offset */
  puts (const_char_ptr);
  puts (const_char_const_ptr);
  puts (static_char_ptr);
  puts (static_const_char_ptr);
  puts (static_const_char_const_ptr);

  puts (char_array);
  puts (const_char_array);
  puts (static_char_array);
  puts (static_const_char_array);

  puts ((an_int == 42) ? "an int: ok" : "an int: value is *NOT* ok");
  puts ((a_const_int == 2*42) ? "a const int: ok" : "a const int: value is *NOT* ok");
  puts ((a_static_int == 3*42) ? "a static int: ok" : "a static int: value is *NOT* ok");
  puts ((a_static_const_int == 4*42) ? "a static const int: ok" : "a static const int: value is *NOT* ok");

  puts (func1());
  puts (func2());

  puts ((*func1_ptr)());
  puts ((*const_func2_ptr)());

  puts ((func1_array[0] == NULL) ? "array function ptr being NULL: ok" : "array function ptr is *NOT* NULL : *NOT* ok");
  puts ((*func1_array[1])());
  puts ((const_func2_array[0] == NULL) ? "const array function ptr being NULL: ok" : "const array function ptr is *NOT* NULL : *NOT* ok");
  puts ((*const_func2_array[1])());

  return EXIT_SUCCESS;
}

_kernel_oserror *
gccmodule_init (const char *cmd_tail, int podule_base, void *pw)
{
  puts ("Testing various ways how data is accessed in module code. We expect to see that each following line is terminated with an 'ok'.\n\n");

  main (0, NULL);

  return NULL;
}

