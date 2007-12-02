/* GCCSDK Module testsuite:
 *
 *   Function pointer calling and non-presence of main().
 * 
 * Written by John Tytgat  <John.Tytgat@aaug.net>
 */

#include <stdio.h>

#include "codecall_fnc.h"
#include "MdlHdr.h"

typedef struct {
  char *name;
  void (*call)(void);
  } callentry_t;

char test1_non_const[] = "test1 non-const: ";

const callentry_t callvector_const[] = {
  { "test1: ", test1 },
  { "test2: ", test2 },
  { "test3: ", test3 },
  { NULL, NULL }
  };

callentry_t callvector_no_const[] = {
  { test1_non_const, test1 },
  { "test2: ", test2 },
  { "test3: ", test3 },
  { NULL, NULL }
  };

_kernel_oserror *
gccmodule_init (const char *cmd_tail, int podule_base, void *pw)
{
  const callentry_t *ce;

  puts ("Testing function pointer calling & non-presence of main() while the module is marked as 'runnable'. We expect to see as output:\n\n"
        "test1: ok1\n"
        "test2: ok2\n"
        "test2: ok3\n"
        "\n"
        "test1 non-const: ok1\n"
        "test2: ok2\n"
        "test2: ok3\n"
        "No main program\n\n"
        "---\n");

  for (ce = callvector_const; ce->name != NULL; ++ce)
    {
    fputs (ce->name, stdout);
//    printf (" %p ", ce->call);
    (*(ce->call)) ();
    }
  fputc ('\n', stdout);
  for (ce = callvector_no_const; ce->name != NULL; ++ce)
    {
    fputs (ce->name, stdout);
//    printf (" %p ", ce->call);
    (*(ce->call)) ();
    }

  return NULL;
}
