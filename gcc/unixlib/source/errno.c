/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/errno.c,v $
 * $Date: 2001/01/29 15:10:18 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: errno.c,v 1.2 2001/01/29 15:10:18 admin Exp $";
#endif

#include <errno.h>

#if !__FEATURE_PTHREADS
int errno = 0;
#endif

