/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/ctype.c,v $
 * $Date: 2000/07/15 14:52:08 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ctype.c,v 1.1.1.1 2000/07/15 14:52:08 nick Exp $";
#endif

#include <ctype.h>

/* ISO C says we must support EOF.  */
static unsigned char ctype[257];
unsigned char *__ctype = &ctype[1];

static short ctype_upper[257];
short *__ctype_upper = &ctype_upper[1];

static short ctype_lower[257];
short *__ctype_lower = &ctype_lower[1];
