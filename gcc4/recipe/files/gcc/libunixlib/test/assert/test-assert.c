/* Test assert().
 *
 * This is hairier than you'd think, involving games with
 * stdio and signals.
 *
 */

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>

jmp_buf rec;
char buf[160];

static void
sigabrt (int unused)
{
  puts ("sigabrt() called"); fflush(stdout);
  longjmp (rec, 1);  /* recover control */
}

#undef NDEBUG
#include <assert.h>
static void
assert1 (void)
{
  assert (1 == 2);
  puts ("assert1() is returning: not ok"); fflush(stdout);
}

static void
assert2 (void)
{
  assert (1 == 1);
  puts ("step 2: ok"); fflush(stdout);
}


#define NDEBUG
#include <assert.h>
static void
assert3 (void)
{
  assert (2 == 3);
  puts ("step 3: ok"); fflush(stdout);
}

int
main (void)
{
  volatile int failed = EXIT_FAILURE;

  fclose (stderr);
  stderr = tmpfile ();
  if (!stderr)
    abort ();

  signal (SIGABRT, sigabrt);

  if (!setjmp (rec))
    assert1 ();
  else
    failed = EXIT_SUCCESS;  /* should happen */

  puts ("step 1: ok"); fflush(stdout);
  if (!setjmp (rec))
    assert2 ();
  else
    failed = EXIT_FAILURE; /* should not happen */

  if (!setjmp (rec))
    assert3 ();
  else
    failed = EXIT_FAILURE; /* should not happen */

  puts ("step 4: ok"); fflush(stdout);
  rewind (stderr);
  fgets (buf, sizeof (buf), stderr);
  if (!strstr (buf, "1 == 2"))
    {
      printf ("failed to find assert1() message in <%s>\n", buf);
      failed = EXIT_FAILURE;
    }

  fgets (buf, sizeof (buf), stderr);
  if (strstr (buf, "1 == 1"))
    {
      printf ("assert2() message wrongly turned up in <%s>\n", buf);
      failed = EXIT_FAILURE;
    }

  fgets (buf, sizeof (buf), stderr);
  if (strstr (buf, "2 == 3"))
    {
      printf ("assert3() message wrongly turned up in <%s>\n", buf);
      failed = EXIT_FAILURE;
    }

  puts ((failed) ? "FAILED" : "SUCCESS");
  return failed;
}
