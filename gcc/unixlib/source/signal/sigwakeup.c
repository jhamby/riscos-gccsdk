/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/sigwakeup.c,v $
 * $Date: 2000/07/15 14:52:30 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigwakeup.c,v 1.1.1.1 2000/07/15 14:52:30 nick Exp $";
#endif

/* Written by Nick Burrett, 26 August 1996.  */

#include <sys/unix.h>

/* Wake a task up.  */
void
sigwakeup (void)
{
  __u->sleeping = 0;
}
