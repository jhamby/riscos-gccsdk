/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/ctype/ctype.c,v $
 * $Date: 2005/04/20 17:12:10 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <ctype.h>

/* ISO C says we must support EOF.  */
static unsigned char ctype[257];
unsigned char *__ctype = &ctype[1];

static short ctype_upper[257];
short *__ctype_upper = &ctype_upper[1];

static short ctype_lower[257];
short *__ctype_lower = &ctype_lower[1];
