/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/strlen,v $
 * $Date: 1997/10/09 16:38:48 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strlen,v 1.4 1997/10/09 16:38:48 unixlib Exp $";
#endif

#include <string.h>

size_t
strlen (const char *start)

{
  const char *end = start - 1;

  while (*++end);

  return end - start;
}
