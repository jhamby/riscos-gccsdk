/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/assert/assert.c,v $
 * $Date: 2005/04/20 17:12:09 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

/* Output something of the form:
      foo.c, line 1234: bar: Assertion failed: a == b
   Then call abort().  */
void
__assert2 (const char *expr, const char *function, const char *file, int line)
{
  PTHREAD_UNSAFE /* Not strictly unsafe, but should help debugging */

  fprintf (stderr, "\n\"%s\", line %d: %s%sAssertion failed: %s\n",
  	   file, line,
  	   function ? function : "",
  	   function ? ": " : "",
  	   expr);
  fflush (stderr);
  abort ();
}

void
(assert) (int expr)
{
  assert (expr);
}
