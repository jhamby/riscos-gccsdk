/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/assert.c,v $
 * $Date: 2003/01/05 12:40:04 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: assert.c,v 1.4 2003/01/05 12:40:04 admin Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

/* Output something of the form:
      foo.c, line 1234: bar: Assertion failed: a == b
   Then call abort().  */
void
__assert_fail (const char *expr, const char *file, int line, const char *function)
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
