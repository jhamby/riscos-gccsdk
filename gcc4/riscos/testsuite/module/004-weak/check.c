/* Sanity check routines for module code generation in GCCSDK.
 * Written by John Tytgat <John.Tytgat@aaug.net>
 */

#include <stddef.h>
#include <stdio.h>

#include "def.h"
#include "MdlHdr.h"

int
main (int argc, char *argv[])
{
  if (argc != 0)
    puts ("\nCalling from module language:");

  fputs ("Test 01/08 : ", stdout);
  if (var_defined == VAR_DEFINED_VALUE)
    puts ("ok");
  else
    puts ("*NOT* ok");

  fputs ("Test 02/08 : ", stdout);
  if (const_var_defined == CONST_VAR_DEFINED_VALUE)
    puts ("ok");
  else
    puts ("*NOT* ok");

  fputs ("Test 03/08 : ", stdout);
  if (func_defined != NULL)
    puts ("ok");
  else
    puts ("*NOT* ok");

  fputs ("Test 04/08 : ", stdout);
  func_defined ();

  fputs ("Test 05/08 : ", stdout);
  if (&var_not_defined == NULL)
    puts ("ok");
  else
    puts ("*NOT* ok (FIXME: known failure)");

  fputs ("Test 06/08 : ", stdout);
  if (&const_var_not_defined == NULL)
    puts ("ok");
  else
    puts ("*NOT* ok (FIXME: known failure)");

  fputs ("Test 07/08 : ", stdout);
  if (func_not_defined == NULL)
    puts ("ok");
  else
    puts ("*NOT* ok");

  fputs ("Test 08/08 : ", stdout);
#if 0
  /* FIXME: this can only be enabled in GCCSDK 4.6.x.  */
  func_not_defined (); /* ARM specific : gets translated to MOV R0,R0 */
#endif
  puts ("ok");

  return 0;
}


_kernel_oserror *
gccmodule_init (const char *cmd_tail, int podule_base, void *pw)
{
  puts ("Testing 'weak' attribute:\n");

  puts ("Calling from module initialisation:");
  main (0, NULL);

  return NULL;
}

