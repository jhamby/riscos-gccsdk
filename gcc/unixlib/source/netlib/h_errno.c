/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/h_errno.c,v $
 * $Date: 2001/01/29 15:10:20 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: h_errno.c,v 1.2 2001/01/29 15:10:20 admin Exp $";
#endif

#include <netdb.h>

int h_errno = 0;
