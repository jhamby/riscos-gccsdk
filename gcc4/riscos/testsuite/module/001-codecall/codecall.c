/* GCCSDK Module testsuite:
 *
 *   Function pointer calling and non-presence of main().
 * 
 * Written by John Tytgat  <John.Tytgat@aaug.net>
 */

#include <stdio.h>

#include "codecall_fnc.h"
#include "MdlHdr.h"

#if 0
#  define dprintf printf
#else
#  define dprintf (1)?0:printf
#endif

typedef void (*testcall_t)(void);

typedef struct
{
  const char *name;
  testcall_t call;
} callentry_t;

char test1_non_const[] = "test1 non-const: ";

const callentry_t callvector_const[] =
{
  { "test1: ", test1 },
  { "test2: ", test2 },
  { "test3: ", test3 },
  { NULL, NULL }
};

callentry_t callvector_no_const[] =
{
  { test1_non_const, test1 },
  { "test2: ", test2 },
  { "test3: ", test3 },
  { NULL, NULL }
};

const testcall_t calltest4 = test4;

testcall_t calltest5 = test5;

testcall_t calltest6 = NULL;

void
SetCallTest6 (testcall_t newVal)
{
  calltest6 = newVal;
}

_kernel_oserror *
gccmodule_init (const char *cmd_tail, int podule_base, void *pw)
{
  const callentry_t *ce;

  CheckInit (pw);

  for (ce = callvector_const; ce->name != NULL; ++ce)
    {
      fputs (ce->name, stdout);
      dprintf (" %p ", ce->call);
      if (CheckForNonWritableRelocNeedingDataPtr (&ce->call)
	  && CheckForCodePtr (ce->call))
        (*(ce->call)) ();
      else
        fputs ("*NOT* ok\n", stdout);
    }
  fputc ('\n', stdout);

  for (ce = callvector_no_const; ce->name != NULL; ++ce)
    {
      fputs (ce->name, stdout);
      dprintf (" %p ", ce->call);
      if (CheckForWritableNonZeroDataPtr (&ce->call)
	  && CheckForCodePtr (ce->call))
        (*(ce->call)) ();
      else
        fputs ("*NOT* ok\n", stdout);
    }
  fputc ('\n', stdout);

  fputs ("test4: ", stdout);
  dprintf (" %p ", calltest4);
  if (CheckForNonWritableRelocNeedingDataPtr (&calltest4)
      && CheckForCodePtr (calltest4))
    (*calltest4) ();
  else
    fputs ("*NOT* ok\n", stdout);

  fputs ("test5: ", stdout);
  dprintf (" %p ", calltest5);
  if (CheckForWritableNonZeroDataPtr (&calltest5)
      && CheckForCodePtr (calltest5))
    (*calltest5) ();
  else
    fputs ("*NOT* ok\n", stdout);

  dprintf ("calltest6: before assignment: %p\n", calltest6);  
  SetCallTest6 (test6);
  fputs ("test6: ", stdout);
  dprintf (" %p ", calltest6);
  if (CheckForWritableZeroDataPtr (&calltest6)
      && CheckForCodePtr (calltest6))
    (*calltest6) ();
  else
    fputs ("*NOT* ok\n", stdout);

  return NULL;
}
