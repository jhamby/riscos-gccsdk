/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id$";
#endif

#include <math.h>
#include <unixlib/math.h>
#include <unixlib/types.h>

int signgam;

/* Defined by POSIX as not threadsafe */
double lgamma (double x)
{
  return lgamma_r (x, &signgam);
}

double gamma (double x)
{
  return lgamma_r (x, &signgam);
}
