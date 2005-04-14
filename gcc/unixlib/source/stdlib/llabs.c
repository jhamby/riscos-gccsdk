/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/abs.c,v $
 * $Date: 2001/01/29 15:10:18 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#include <stdlib.h>

long long llabs (long long x)
{
  return ((x < 0) ? -x : x);
}
