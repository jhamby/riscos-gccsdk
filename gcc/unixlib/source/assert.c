/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/assert,v $
 * $Date: 1997/10/08 16:37:44 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: assert,v 1.4 1997/10/08 16:37:44 unixlib Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Output something of the form:
      foo.c, line 1234: bar: Assertion failed: a == b
   Then call abort().  */
void
__assert_fail (char *exp, const char *file, int line, const char *function)
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
