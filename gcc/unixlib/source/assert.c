/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/assert.c,v $
 * $Date: 2001/08/09 07:48:03 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: assert.c,v 1.2.2.1 2001/08/09 07:48:03 admin Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Output something of the form:
      foo.c, line 1234: bar: Assertion failed: a == b
   Then call abort().  */
void
__assert_fail (const char *exp, const char *file, int line, const char *function)
{
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
