/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/math/c/lgamma,v $
 * $Date: 1997/09/07 16:21:53 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: lgamma,v 1.1 1997/09/07 16:21:53 unixlib Exp $";
#endif

#include <math.h>
#include <unixlib/math.h>
#include <unixlib/types.h>

int signgam;

double lgamma (double x)
{
  return lgamma_r (x, &signgam);
}

double gamma (double x)
{
  return lgamma_r (x, &signgam);
}
