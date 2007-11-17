/* GCCSDK Module testsuite:
 *
 *   Function pointer calling and non-presence of main().
 * 
 * Written by John Tytgat  <John.Tytgat@aaug.net>
 */

#include <stdio.h>

#include "MdlHdr.h"

static void test1 (void);
static void test2 (void);
static void test3 (void);

typedef struct {
  const char *name;
  void (*call)(void);
  } callentry_t;

const callentry_t callvector[] = {
  { "test1: ", test1 },
  { "test2: ", test2 },
  { "test3: ", test3 },
  { NULL, NULL }
  };

static void
test1 (void)
{
  puts("ok1");
}

static void
test2 (void)
{
  puts("ok2");
}

static void
test3 (void)
{
  puts("ok3");
}

_kernel_oserror *
gccmodule_init (const char *cmd_tail, int podule_base, void *pw)
{
  const callentry_t *ce;

  puts ("Testing function pointer calling & non-presence of main() while the module is marked as 'runnable'. We expect to see as output:\n\n"
        "test1: ok1\n"
        "test2: ok2\n"
        "test2: ok3\n"
        "No main program\n\n"
        "---\n\n");

  for (ce = callvector; ce->name != NULL; ++ce)
    {
    fputs (ce->name, stdout);
    (*(ce->call)) ();
    }

  return NULL;
}

