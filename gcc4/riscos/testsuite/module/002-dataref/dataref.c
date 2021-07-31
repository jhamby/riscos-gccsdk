/* GCCSDK Module testsuite:
 *
 *   Test data relocation at module initialisation time and main() time.
 * 
 * Written by John Tytgat  <John.Tytgat@aaug.net>
 */

#include <stdio.h>
#include <stdlib.h>

#include "check.h"
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
  const int *value;
} field_t;

opt_t opt;
const opt_t const_opt =
{
  0x78,
  0x9A,
  0xBC
};

field_t field[] =
{
  { "field: abc: ", &opt.abc },
  { "field: def: ", &opt.def },
  { "field: ghi: ", &opt.ghi },
  { "field: const abc: ", &const_opt.abc },
  { "field: const def: ", &const_opt.def },
  { "field: const ghi: ", &const_opt.ghi },
};

const field_t const_field[] =
{
  { "const_field: abc: ", &opt.abc },
  { "const_field: def: ", &opt.def },
  { "const_field: ghi: ", &opt.ghi },
  { "const_field: const abc: ", &const_opt.abc },
  { "const_field: const def: ", &const_opt.def },
  { "const_field: const ghi: ", &const_opt.ghi },
};

int
main (int argc, char *argv[])
{
  CheckForWritableZeroDataPtr (&opt);
  CheckForNonWritableDataPtr (&const_opt);
  CheckForWritableNonZeroDataPtr (&field);
  CheckForNonWritableRelocNeedingDataPtr (&const_field);

  opt.abc = 0x12;
  opt.def = 0x34;
  opt.ghi = 0x56;

  /* Non const field.  */
  fputs (field[0].name, stdout);
  if (field[0].value == &opt.abc
      && *field[0].value == 0x12)
    fputs ("ok\n", stdout);
  else
    fputs ("*NOT* ok\n", stdout);
  fputs (field[1].name, stdout);
  if (field[1].value == &opt.def
      && *field[1].value == 0x34)
    fputs ("ok\n", stdout);
  else
    fputs ("*NOT* ok\n", stdout);
  fputs (field[2].name, stdout);
  if (field[2].value == &opt.ghi
      && *field[2].value == 0x56)
    fputs ("ok\n", stdout);
  else
    fputs ("*NOT* ok\n", stdout);
  fputs (field[3].name, stdout);
  if (field[3].value == &const_opt.abc
      && *field[3].value == 0x78)
    fputs ("ok\n", stdout);
  else
    fputs ("*NOT* ok\n", stdout);
  fputs (field[4].name, stdout);
  if (field[4].value == &const_opt.def
      && *field[4].value == 0x9A)
    fputs ("ok\n", stdout);
  else
    fputs ("*NOT* ok\n", stdout);
  fputs (field[5].name, stdout);
  if (field[5].value == &const_opt.ghi
      && *field[5].value == 0xBC)
    fputs ("ok\n", stdout);
  else
    fputs ("*NOT* ok\n", stdout);

  /* Const field.  */
  fputs (const_field[0].name, stdout);
  if (const_field[0].value == &opt.abc /* FIXME: known to fail atm.  */
      && *const_field[0].value == 0x12)
    fputs ("ok\n", stdout);
  else
    fputs ("*NOT* ok\n", stdout);
  fputs (const_field[1].name, stdout);
  if (const_field[1].value == &opt.def /* FIXME: known to fail atm.  */
      && *const_field[1].value == 0x34)
    fputs ("ok\n", stdout);
  else
    fputs ("*NOT* ok\n", stdout);
  fputs (const_field[2].name, stdout);
  if (const_field[2].value == &opt.ghi /* FIXME: known to fail atm.  */
      && *const_field[2].value == 0x56)
    fputs ("ok\n", stdout);
  else
    fputs ("*NOT* ok\n", stdout);
  fputs (const_field[3].name, stdout);
  if (const_field[3].value == &const_opt.abc
      && *const_field[3].value == 0x78)
    fputs ("ok\n", stdout);
  else
    fputs ("*NOT* ok\n", stdout);
  fputs (const_field[4].name, stdout);
  if (const_field[4].value == &const_opt.def
      && *const_field[4].value == 0x9A)
    fputs ("ok\n", stdout);
  else
    fputs ("*NOT* ok\n", stdout);
  fputs (const_field[5].name, stdout);
  if (const_field[5].value == &const_opt.ghi
      && *const_field[5].value == 0xBC)
    fputs ("ok\n", stdout);
  else
    fputs ("*NOT* ok\n", stdout);

  return EXIT_SUCCESS;
}

_kernel_oserror *
gccmodule_init (const char *cmd_tail, int podule_base, void *pw)
{
  CheckInit (pw);

  puts ("Testing if data relocation works at module initialisation and main() time.\n\n");

  main (0, NULL);
  fputc ('\n', stdout);

  return NULL;
}
