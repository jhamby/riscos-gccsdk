/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id$";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

/* Output something of the form:
      foo.c, line 1234: bar: Assertion failed: a == b
   Then call abort().  */
void
__assert_fail (const char *exp, const char *file, int line, const char *function)
{
  PTHREAD_UNSAFE /* Not strictly unsafe, but should help debugging */

  fprintf (stderr, "\n\"%s\", line %d: %s%sAssertion failed: %s\n",
  	   file, line,
  	   function ? function : "",
  	   function ? ": " : "",
  	   exp);
  fflush (stderr);
  abort ();
}

void
(assert) (int expr)
{
  assert (expr);
}
