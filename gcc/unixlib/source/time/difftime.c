/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/time/c/difftime,v $
 * $Date: 1997/10/09 20:00:45 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: difftime,v 1.2 1997/10/09 20:00:45 unixlib Exp $";
#endif

#include <time.h>

double
difftime (time_t time1, time_t time0)
{
  return (double)time1 - (double)time0;
}
