/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/ctypeproc.c,v $
 * $Date: 2001/09/14 14:01:17 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ctypeproc.c,v 1.2.2.1 2001/09/14 14:01:17 admin Exp $";
#endif

#include <ctype.h>

int
(toascii) (int c)
{
  return toascii (c);
}

int
(tolower) (int c)
{
  return tolower (c);
}

int
(toupper) (int c)
{
  return toupper (c);
}

int
(_tolower) (int c)
{
  return _tolower (c);
}

int
(_toupper) (int c)
{
  return _toupper (c);
}

int
  (isascii) (int c)
{
  return isascii (c);
}

int
  (isalnum) (int c)
{
  return (isalnum (c));
}

int
  (isalpha) (int c)
{
  return isalpha (c);
}

int
  (iscntrl) (int c)
{
  return iscntrl (c);
}

int
  (isdigit) (int c)
{
  return isdigit (c);
}

int
  (isgraph) (int c)
{
  return isgraph (c);
}

int
  (islower) (int c)
{
  return islower (c);
}

int
  (isprint) (int c)
{
  return isprint (c);
}

int
  (ispunct) (int c)
{
  return ispunct (c);
}

int
  (isspace) (int c)
{
  return isspace (c);
}

int
  (isupper) (int c)
{
  return isupper (c);
}

int
  (isxdigit) (int c)
{
  return isxdigit (c);
}
