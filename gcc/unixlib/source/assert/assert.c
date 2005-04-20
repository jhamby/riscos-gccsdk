/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/assert.c,v $
 * $Date: 2004/04/20 20:20:36 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: assert.c,v 1.6 2004/04/20 20:20:36 joty Exp $";
#endif

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
