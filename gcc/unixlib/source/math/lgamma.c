/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math/lgamma.c,v $
 * $Date: 2000/07/15 14:52:21 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: lgamma.c,v 1.1.1.1 2000/07/15 14:52:21 nick Exp $";
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
