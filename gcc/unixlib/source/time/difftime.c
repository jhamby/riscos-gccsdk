/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/difftime.c,v $
 * $Date: 2000/07/15 14:52:42 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: difftime.c,v 1.1.1.1 2000/07/15 14:52:42 nick Exp $";
#endif

#include <time.h>

double
difftime (time_t time1, time_t time0)
{
  return (double)time1 - (double)time0;
}
