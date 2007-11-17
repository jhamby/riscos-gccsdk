/* GCCSDK Module testsuite:
 *
 *   Test data relocation at module initialisation time and main() time.
 * 
 * Written by John Tytgat  <John.Tytgat@aaug.net>
 */

#include <stdio.h>

#include "MdlHdr.h"

typedef struct
{
  int abc;
  int def;
  int ghi;
} opt_t;

typedef struct
{
  char *name;
  int *value;
} field_t;

opt_t opt;

field_t field[] =
  {
  { "abc", &opt.abc },
  { "def", &opt.def },
  { "ghi", &opt.ghi }
  };

int
main (int argc, char *argv[])
{
  opt.abc = 0x12;
  opt.def = 0x34;
  opt.ghi = 0x56;

  printf ("opt %p, field %p\n", &opt, &field);
  
  fputs (field[0].name, stdout); printf(" : %p, %p : 0x%x\n", &opt.abc, field[0].value, *field[0].value);
  fputs (field[1].name, stdout); printf(" : %p, %p : 0x%x\n", &opt.def, field[1].value, *field[1].value);
  fputs (field[2].name, stdout); printf(" : %p, %p : 0x%x\n", &opt.ghi, field[2].value, *field[2].value);
  
  return 0;
}

_kernel_oserror *
gccmodule_init (const char *cmd_tail, int podule_base, void *pw)
{
  puts ("Testing if data relocation works at module initialisation and main() time. We expect to see similar output for those two cases. Also the line beginning with 'abc' should end with '0x12', the one with 'def' with '0x34' and the one with 'ghi' with '0x56'.\n\n");

  main (0, NULL);
  fputc ('\n', stdout);

  return NULL;
}

