/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/c/ctype,v $
 * $Date: 1997/10/08 12:48:08 $
 * $Revision: 1.6 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ctype,v 1.6 1997/10/08 12:48:08 unixlib Exp $";
#endif

#include <ctype.h>

/* ISO C says we must support EOF.  */
static unsigned char ctype[257];
unsigned char *__ctype = &ctype[1];

static short ctype_upper[257];
short *__ctype_upper = &ctype_upper[1];

static short ctype_lower[257];
short *__ctype_lower = &ctype_lower[1];
