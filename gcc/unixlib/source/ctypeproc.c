/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/ctypeproc,v $
 * $Date: 1997/10/08 12:48:08 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ctypeproc,v 1.1 1997/10/08 12:48:08 unixlib Exp $";
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
