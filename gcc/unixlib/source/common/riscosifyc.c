/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/common/riscosifyc.c,v $
 * $Date: 2001/01/29 15:10:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: riscosifyc.c,v 1.2 2001/01/29 15:10:19 admin Exp $";
#endif

#include <unixlib/local.h>

/* Default value for riscosify().  This is a weak symbol so it can be
 * overruled in an user program.  */
int __riscosify_control = 0;
